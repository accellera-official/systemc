README for Pipe
=================================================================
 
This is a basic example to show how to use SystemC and user
can use it as a template to create more complex models.
 
It consists of 3 processes that form individual stages of a pipleline.
 
| +-----------+     +-----------+   +-----------+  |
| |           |     |           |   |           |  |
| | stage1    |---->| stage2    |-->| stage3    |  |
| |           |     |           |   |           |  |
| +-----------+     +-----------+   +-----------+  |
 
The first stage of the pipeline accepts 2 inputs and computes their sum
and difference.  The second stage accepts the results of the first
stage and computes their product and quotient.  Finally stage3 
accepts these outputs from second stage and computes the first input raised to
the power of the second.
 
In order to compile this model, you have to execute gmake/make,
then you should find a run.x executable after the compilation. Execute
run.x and it will print out the results to your screen.
