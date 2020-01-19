import cv2
import numpy as np
import imutils


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


def cannyHough():
    img = cv2.imread('../oriental_picture.png')
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    gray = cv2.bilateralFilter(gray, 5, 100, 50)
    edges = cv2.Canny(gray, 50, 120)

    rectLines = []
    shape = "uninitialized"
    minLineLength = 50
    maxLineGap = 10
    lines = cv2.HoughLinesP(edges, 1, np.pi / 180, 20, minLineLength, maxLineGap)
    print(len(lines))
    for i in range(0, 2):
        for x1, y1, x2, y2 in lines[i]:
            cv2.line(img, (x1, y1), (x2, y2), (0, 255, 255), 2)
            rectLines.append(([x1, y1], [x2, y2]))

    # Checking if lines intersect to form a rectangle or square
    if rectLines[0][0][0] == rectLines[1][0][0] and rectLines[0][1][0] == rectLines[1][1][0]:
        if (rectLines[1][0][1] - rectLines[0][0][1]) != (rectLines[0][0][1] - rectLines[0][0][0]):
            shape = "Rectangle"
        else:
            shape = "Square"

    cv2.imshow("edges", edges)
    cv2.imshow("lines", img)
    cv2.waitKey()
    cv2.destroyAllWindows()


def detect(c):
    shape = "unidentified"
    peri = cv2.arcLength(c, True)
    approx = cv2.approxPolyDP(c, 0.04 * peri, True)

    if len(approx) == 3:
        shape = "triangle"

    elif len(approx) == 4:
        # compute the bounding box of the contour and use the bounding box to compute the aspect ratio
        (x, y, w, h) = cv2.boundingRect(approx)
        ar = w / float(h)

        coordinates = [x, y, (x + w), (h + y)]

        # a square will have an aspect ratio that is approximately equal to one, otherwise, the shape is a rectangle
        shape = "square" if ar == 1 else "rectangle"

    elif len(approx) == 5:
        shape = "pentagon"

    else:
        shape = "circle"

    return shape, coordinates


def redDetect():
    img = cv2.imread('../oriental_picture.png')
    boundaries = [([0, 0, 0], [0, 0, 255])]

    lower = np.array(boundaries[0][0], dtype="uint8")
    upper = np.array(boundaries[0][1], dtype="uint8")

    # find the colors within the specified boundaries and apply the mask
    mask = cv2.inRange(img, lower, upper)
    output = cv2.bitwise_and(img, img, mask=mask)

    # convert the resized image to grayscale, blur it slightly, and threshold it
    gray = cv2.cvtColor(output, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]

    # find contours in the thresholded image and initialize the shape detector
    cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
                            cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)

    # loop over the contours
    for c in cnts:
        shape, coordinates = detect(c)
        print(shape, coordinates)
        c = c.astype("int")
        cv2.drawContours(output, [c], -1, (0, 255, 0), 2)

    cv2.imshow("Image", output)
    cv2.waitKey(0)


if __name__ == '__main__':
    print("We go again!!")
    CircleDetection()
    cannyHough()
    redDetect()
