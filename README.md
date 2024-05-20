# VectorPool

In some applications it is advantageous to have a contiguous pool of data in memory, but it is still necessary to
modify portions of that data. A good example of this is if you had data that is continuously being fed into a pipeline of some sort, 
and the data was modified less often than if it was being fed into the pipeline, such as objects having physics performed on them in an engine.

This is a (very) naive approach to this idea, made to test this idea.
