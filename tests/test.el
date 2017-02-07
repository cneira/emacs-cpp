(add-to-list 'load-path "./")
(require 'mymod)
(mymod-test "t2.cc"  4 5)
(mymod-test "t3.cc"  4 5)
(defvar v (mymod-test "sample2_l6c9.cc"  6 9))
v 
