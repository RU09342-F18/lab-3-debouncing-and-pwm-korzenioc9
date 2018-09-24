# Lab 3: Timers and PWM
In this lab, you will begin to use one of the most fundamental control techniques for a microcontroller. You can blink an LED, and you can blink it at a particular rate, but so far you have done symmetric waves. This means that you have you output be on and off for the same amount of time. What do you think will happen if you, say, keep the LED turned off for only 10% of the time instead of 50%? And what about those buttons, have you already had issues where your button accidentally triggers your code multiple times with interrupts enabled?

## Goals for the lab:
By the end of this lab, you should:
* Debounce a switch input using the Timer Modules and interrupts
* Implement Software based PWM using the built in Timer Modules
* Implement Hardware based PWM using P1SEL and the timer modules

## Deliverables
By the end of the lab, you will need to provide at a minimum well documented main.c files for *EACH* of the development platforms with accompanying README files which explain exactly how to implement your code. The reason I say minimum is because there are going to be some recommended further practice that is not mandatory, but heavily recommended, especially if you are finishing this lab in only a couple hours. So come Milestone time, I should see 3 folders in your repository with the following titles:
* Debouncing
* Software PWM
* Hardware PWM

each with the .c and README files for each board. The .c and README files should include the minimum amount of work as well as any extra work that you have done. Each processor should have its project saved in its own folder in each part of the assignment, however you only need one README per part of the assignment.

### README Files
Since most of these projects will have just a simple main.c file, you do not need to generate 20 README files. Instead, unless you go for a more advanced implementation of the exercises, you just need 1 README per exercise folder. "But how do I make a README with all of the processors included?" Well now we are getting somewhere. You should talk about the general form of your code as it should be very similar for each processor, but you should highlight what the differences are between each processor. For example, do the clocks need to be initialized differently? As another step forward, you could take that information and somehow make it where your code would work on any of the processors without the need to change it between projects.

### Header Files
You may find yourself by the end of this lab not having generated any header files, which in this case, ignore this section. If you have generated more than just a main.c you most likely have yourself a .h file or two. Remember from the first lab that any header files or libraries that you generate need to have their own README.md which tell the user what dependencies there are, how to install them into their projects, etc.

## Documentation
Since you will most likely be using pre-made code to make most of your code, so what I am going to require you to do is comment each line of code and what it is doing. If you are using code from TI themselves, it will most likely have some of the comments there. But I want you to use your own words when writing these comments. Dive into the datasheets, look into the MSP430.h file to see what registers are being manipulated and tell me why you need to configure them.
