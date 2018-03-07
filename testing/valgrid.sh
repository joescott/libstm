valgrind --tool=memcheck --leak-check=yes --leak-check=full --leak-resolution=high --show-reachable=yes --num-callers=20 --track-fds=yes --track-origins=yes -v ./test
