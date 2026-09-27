In the delimiter checker, each stack element is waiting for a closer. For example, if you push  ((( you will later have to give  ))) in order to properly end the stack. The postfix evaluator has each item until it is used. When you feed it a number, you are telling it to hold it for later use. 

For the Delimiter, the reason we need the most recent pushed item is that we close the nest from the inside out. The closer is always in the innermost nest, and as such, it requires the Lifo principle to properly run.

Postfix requires it in order to correctly do the math we expect of it. If we do not follow LIFO we will get a complete incorrect answer as the math will be done wrong.

Delimiter example: (\[)\]  
Push (, then push \[.  
) arrives. Pop the top, which is \[, but ) needs (. That's a mismatch, so it's an error.  
This shows LIFO is the rule for nesting: the most recent opener must close first.

Postfix example: 10 4 \-  
Push 10, then push 4\.  
\- arrives. The first pop gives 4 (right) and the second gives 10 (left).  
So the answer is 10 − 4 \= 6\. Swap the order and you get −4.  
This shows the stack's reverse order matters, and you have to account for it.