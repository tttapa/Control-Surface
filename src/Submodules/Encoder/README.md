# Encoder Library

This library is a fork of PJRC's popular Encoder library
(http://www.pjrc.com/teensy/td_libs_Encoder.html).

I applied the following changes:
 - Delete the copy constructor and copy assignment operator: in the original
   library, the default copy constructor doesn't work, and leaves dangling 
   pointers to the interrupt contexts when used accidentally.
 - Implement the move constructor and move assignment operator: this solves the
   dangling pointers problem mentioned earlier, and allows passing encoders to
   other classes without having to construct it in-place. This requires 
   functions that can detach and re-attach the interrupts correctly, updating
   the ISR context pointers.
 - Add a code generator to automatically generate the many repetitive ISRs:
   a Python script generates a huge switch statement that returns the right 
   lambda function to be called in the ISR.
