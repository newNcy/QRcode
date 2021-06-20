# QRcode

qr二维码编码库

https://www.thonky.com/qr-code-tutorial/data-analysis

# 解码
## 预处理
### 灰度化
gray = r*0.299+ g*587 + b*0.114 + 0.5
### 二值化
#### 固定阈值二值化
bin = gray > threshold ? 255 : 0
#### 基于直方图二值化
取最大最小中间值
#### 自适应阈值二值化
根据文章：!(Adaptive Thresholding Using the Integral Image)[http://http//www.ppgia.pucpr.br/~facon/Binarizacao/1993thresholdAdaptativeWellner.pdf]的方法





