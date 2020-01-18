import cv2
import numpy as np


def CircleDetection():
    img = cv2.imread('../oriental_picture.png', 0)
    img = cv2.medianBlur(img, 5)
    cimg = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)

    circles = cv2.HoughCircles(img, cv2.HOUGH_GRADIENT, 1, 160,
                               param1=20, param2=50, minRadius=40, maxRadius=90)

    circles = np.uint16(np.around(circles))
    for i in circles[0, :]:
        # draw the outer circle
        cv2.circle(cimg, (i[0], i[1]), i[2], (0, 255, 0), 2)
        # draw the center of the circle
        cv2.circle(cimg, (i[0], i[1]), 2, (0, 0, 255), 3)

    points = [i[0], i[1], i[2]]
    cv2.imshow('detected circles', cimg)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    computeCircleEnclosure(points)


def computeCircleEnclosure(circleData):
    cx = circleData[0]
    cy = circleData[1]
    r = circleData[2]

    x1 = ((cx - r), (cy - r))
    x2 = ((cx + r), (cy + r))

    print(x1, x2)


if __name__ == '__main__':
    print("We go again!!")
    CircleDetection()
