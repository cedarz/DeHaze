


# DeHaze

【1】[Single Image Haze Removal Using Dark Channel Prior](http://research.microsoft.com/en-us/um/people/jiansun/papers/Dehaze_CVPR2009.pdf)

【2】[A Closed Form Solution to Natural Image Matting](http://www.wisdom.weizmann.ac.il/~levina/papers/Matting-Levin-Lischinski-Weiss-CVPR06.pdf)

## 完成情况

实现起来真正的难点在soft matting，当然作者后来有一个文章是[Guided Image Fileter] (http://research.microsoft.com/en-us/um/people/kahe/eccv10/index.html) 可以用来解决该问题，好像更加高效。

- 暗通道先验图 【done】
- medium transmission矩阵 【done】
- soft matting/软抠图 【】
- 代替soft matting 使用了guided image filter的实现，开源【done】
- 评估大气光/atmospheric light 【done】
- 去雾操作 【done】

## 测试

- [测试图片](http://kaiminghe.com/cvpr09/results.html)

## git->github

- [如何使用Git上传项目代码到github](http://1ke.co/course/194)


## 参考

以下文章或多或少给我了验证想法和实现效果的参照，特此感谢

 - [Natural Image Matting](http://cs.brown.edu/courses/cs129/results/final/valayshah/)
 - [
学习何凯明《Single Image Haze Removal Using Dark Channel Prior》一文的总结（高清无码)](http://www.cnblogs.com/bbsno1/p/3279913.html)
 - [图像去雾新进展](http://yueyue1105.blog.163.com/blog/static/4311176820101169534756/)
 - [图像去雾成果](http://yueyue1105.blog.163.com/blog/static/43111768201011160563445/)
 - [《Single Image Haze Removal Using Dark Channel Prior》一文中图像去雾算法的原理、实现、效果（速度可实时）](http://www.cnblogs.com/Imageshop/p/3281703.html)

## 结果

- 结果并不很理想，容易出现大量的五颜六色的色斑
- 有一张测试图中的arilight大量候选点是255，可能有关系

## 感谢

- 代码中使用了[atilimcetin/guided-filter](https://github.com/atilimcetin/guided-filter)的`Guided Image Filter`的实现,包含 guidedfilter.cpp和 guidedfilter.h
