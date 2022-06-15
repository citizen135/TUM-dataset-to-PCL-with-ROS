# ORB로 특징점 및 특징 디스크립터 검출 (desc_orb.py)

import cv2
import numpy as np

img1 = cv2.imread('/home/pjh/data/rgbd_dataset_freiburg2_desk/rgb/1311868164.363181.png')
img2 = cv2.imread('/home/pjh/data/rgbd_dataset_freiburg2_desk/rgb/1311868164.863058.png')
gray1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)

# ORB 추출기 생성
orb = cv2.ORB_create()
# 포인트 검출과 서술자 계산
keypoints1, descriptor1 = orb.detectAndCompute(gray1, None)
keypoints2, descriptor2 = orb.detectAndCompute(gray2, None)

matcher = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck = True)
match = matcher.match(descriptor1, descriptor2)
print(match)

# 포인트 그리기
img_draw = cv2.drawMatches(img1, keypoints1, img2, keypoints2, match, None, \
             flags=cv2.DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS)
# 결과 출력
cv2.imshow('BFMatcher & ORB', img_draw)
cv2.waitKey()
cv2.destroyAllWindows()
