import cv2


def Camera():
    video = 0

    # comment next line if you want the webcam to work
    video = "C:\\Users\\AOZ\\Desktop\\test3.mp4"

    if not (video):
        cam = cv2.VideoCapture(0)
    else:
        cam = cv2.VideoCapture(video)

    cam.set(3, 680)
    cam.set(4, 480)

    classNames = []
    classFile = 'objects.names'

    with open(classFile, 'rt') as f:
        classNames = f.read().rstrip('\n').split('\n')

    configPath = 'tensorflow_readable.pbtxt'
    weightpath = 'tensorflow_binary_graph.pb'

    net = cv2.dnn_DetectionModel(weightpath, configPath)
    net.setInputSize(320, 230)
    net.setInputScale(1.0 / 127.5)
    net.setInputMean((127.5, 127.5, 127.5))
    net.setInputSwapRB(True)

    while True:
        success, img = cam.read()

        # if video ended then loop || if camera not attached then break
        if not success:
            if not (video):
                break
            else:
                # break
                cam = cv2.VideoCapture(video)
                success, img = cam.read()

        # detect the objects and thier location and id
        classIds, confs, bbox = net.detect(img, confThreshold=0.5)
        # print(classIds, bbox)

        if len(classIds) != 0:
            for classId, confidence, box in zip(classIds.flatten(), confs.flatten(), bbox):
                if classNames[classId-1] == "ball":
                    cv2.rectangle(img, box, color=(0, 255, 0), thickness=2)
                    cv2.putText(img, classNames[classId-1], (box[0] + 10, box[1] + 20),
                                cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), thickness=2)
                    print(classNames[classId-1], "x =" + str(box[0]+box[2]/2),
                          "y = " + str(box[1]+box[3]/2), "radius = " + str(box[2]/2))

        cv2.imshow('Ball Detection', img)
        key = cv2.waitKey(1) & 0xFF
        # if the 'q' key is pressed, stop the loop
        if key == ord("q"):
            break
######################################


Camera()
