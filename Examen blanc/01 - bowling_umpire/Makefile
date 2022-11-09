all: bowling_team bowling_umpire

bowling_umpire: bowling_umpire.c socket_conf.h
	gcc -g -o bowling_umpire bowling_umpire.c

bowling_team: bowling_team.c socket_conf.h
	gcc -g -o bowling_team bowling_team.c

clean:
	rm bowling_umpire bowling_team
