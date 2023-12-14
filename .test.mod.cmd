cmd_/home/zaqmond/Desktop/time/test.mod := printf '%s\n'   test.o | awk '!x[$$0]++ { print("/home/zaqmond/Desktop/time/"$$0) }' > /home/zaqmond/Desktop/time/test.mod
