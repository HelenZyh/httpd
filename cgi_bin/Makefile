FIND_PATH=-I/home/helen/Project/httpd/sql_connect -I/home/helen/Project/httpd/sql_connect/mysql_lib/include -L/home/helen/Project/httpd/sql_connect/mysql_lib/lib -lmysqlclient

.PHONY:all
all:math_cgi debug_cgi show_student_cgi register_cgi

debug_cgi:debug_cgi.c
	gcc -o $@ $^

math_cgi:math_cgi.c
	gcc -o $@ $^

show_student_cgi:show_student_cgi.cpp /home/helen/Project/httpd/sql_connect/sql_connect.cpp
	g++ -o $@ $^ $(FIND_PATH) -g

register_cgi:register_cgi.cpp /home/helen/Project/httpd/sql_connect/sql_connect.cpp
	g++ -o $@ $^ $(FIND_PATH) 
.PHONY:output
output:
	cp math_cgi ../output/htdocs/cgi_bin
	cp debug_cgi ../output/htdocs/cgi_bin
	cp show_student_cgi ../output/htdocs/cgi_bin
	cp register_cgi ../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf math_cgi debug_cgi  show_student_cgi register_cgi
