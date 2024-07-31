uname
echo 1 2 3 > file && wc < file &
echo 1 2 3 | wc
echo 1 >> file || echo 2 >> file && cat file
echo 1 2 3 | wc > file; cat file
echo 1 || (echo 2 && echo 3)
yes | head
