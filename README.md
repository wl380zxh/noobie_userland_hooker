Learning how to make userland hooks.

This programd does follwing:
 
1) Get address of called function (createProcessA);
2) Create hook function (hookedCreateprocess);
3) Rewrite address of standart CreateProcessA function to our custom function (inside of custom function different createprocess is called);
4) hooked function is executed.
