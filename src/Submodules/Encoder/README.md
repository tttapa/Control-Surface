# Encoder Library

This library is an adapted and modernized version of PJRC's popular Encoder
library (http://www.pjrc.com/teensy/td_libs_Encoder.html).

It includes following changes:
 - Delete the copy constructor and copy assignment operator: in the original
   library, the default copy constructor doesn't work, and leaves dangling 
   pointers to the interrupt contexts when used accidentally.
 - Implement the move constructor and move assignment operator: this solves the
   dangling pointers problem mentioned earlier, and allows passing encoders to
   other classes without having to construct it in-place. This requires 
   functions that can detach and re-attach the interrupts correctly, updating
   the ISR context pointers.
 - Use templates to automatically generate the many repetitive ISRs.
