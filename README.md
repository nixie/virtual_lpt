virtual\_lpt
===========

Small app to visualize byte sequences throuhg unix named-pipes

1. Run ./vp to open window and create "input" named pipe.
2. Write some bytes to it `echo -n 'a' > input"`
3. End session by sending zero to input `"echo '\0' > input"`
4. Display string sequentialy in binary:

    for c in \`echo "Hello world!" | sed -e 's/\(.\)/\1\n/g'\`;do
    echo -n $c > input; sleep 1
    done
