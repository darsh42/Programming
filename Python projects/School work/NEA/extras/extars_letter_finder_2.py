import cv2

image = cv2.imread(r"images\readin\default_text_img.jpg")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

thresh, dst = cv2.threshold(gray, 70, 255, cv2.THRESH_OTSU)
blurred = cv2.GaussianBlur(dst, (5, 5), 0)
cv2.imshow('win',dst)
cv2.imshow('thresh', thresh)
  
edged = cv2.Canny(blurred, 30, 150)
cnts = cv2.findContours(edged.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)


chars = []


for c in cnts:
    (x, y, w, h) = cv2.boundingRect(c)
    if (w >= 5 and w <= 150) and (h >= 15 and h <= 120):

        roi = gray[y:y + h, x:x + w]
        thresh = cv2.threshold(roi, 0, 255,
            cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)[1]
        (tH, tW) = thresh.shape
        
        # if the width is greater than the height, resize along the
        # width dimension
        if tW > tH:
            thresh = imutils.resize(thresh, width=28)
        # otherwise, resize along the height
        else:
            thresh = imutils.resize(thresh, height=28)
        # re-grab the image dimensions (now that its been resized)
        # and then determine how much we need to pad the width and
        # height such that our image will be 32x32
        (tH, tW) = thresh.shape
        dX = int(max(0, 28 - tW) / 2.0)
        dY = int(max(0, 28 - tH) / 2.0)
        # pad the image and force 32x32 dimensions
        padded = cv2.copyMakeBorder(thresh, top=dY, bottom=dY,
            left=dX, right=dX, borderType=cv2.BORDER_CONSTANT,
            value=(0, 0, 0))
        padded = cv2.resize(padded, (28, 28))
        # prepare the padded image for classification via our
        # handwriting OCR model
        padded = padded.astype("float32") / 255.0
        padded = np.expand_dims(padded, axis=-1)
        # update our list of characters that will be OCR'd
        chars.append((padded, (x, y, w, h)))