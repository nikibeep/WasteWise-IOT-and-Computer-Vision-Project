# import the necessary packages
from flask import Response, Flask, render_template
import threading
import argparse
import datetime, time
import imutils
import cv2

# initialize the output frame and a lock used to ensure thread-safe
# exchanges of the output frames (useful when multiple browsers/tabs are viewing the stream)
outputFrame = None
lock = threading.Lock()

# initialize a flask object
app = Flask(__name__)

# Replace the source with the appropriate RTSP stream URL
source = "rtsp://192.168.137.134:8080/h264.sdp"
cap = cv2.VideoCapture(source)
time.sleep(2.0)

@app.route("/")
def index():
    # return the rendered template
    return render_template("index.html")

def stream(frameCount):
    global outputFrame, lock
    if cap.isOpened():
        while True:
            ret_val, frame = cap.read()
            if frame is not None:
                # Resize the frame to a common size for web streaming
                frame = cv2.resize(frame, (640, 360))
                with lock:
                    outputFrame = frame.copy()
            else:
                continue
    else:
        print('Camera open failed')

def generate():
    # grab global references to the output frame and lock variables
    global outputFrame, lock

    # loop over frames from the output stream
    while True:
        # wait until the lock is acquired
        with lock:
            # check if the output frame is available, otherwise skip
            # the iteration of the loop
            if outputFrame is None:
                continue

            # encode the frame in JPEG format
            (flag, encodedImage) = cv2.imencode(".jpg", outputFrame)

            # ensure the frame was successfully encoded
            if not flag:
                continue

        # yield the output frame in the byte format
        yield (b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' +
               bytearray(encodedImage) + b'\r\n')

@app.route("/video_feed")
def video_feed():
    # return the response generated along with the specific media
    # type (mime type)
    return Response(generate(),
                    mimetype="multipart/x-mixed-replace; boundary=frame")

if __name__ == '__main__':
    # start the Flask app
    app.run(host='0.0.0.0', port=8080, debug=True, threaded=True, use_reloader=False)

# release the video stream pointer
cap.release()
cv2.destroyAllWindows()
