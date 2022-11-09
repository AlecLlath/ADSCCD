all: password_whisperer vault word_of_mouth_player

password_whisperer: password_whisperer.c socket_conf.h
	gcc -o password_whisperer password_whisperer.c

vault: vault.c socket_conf.h
	gcc -o vault vault.c

word_of_mouth_player: word_of_mouth_player.c socket_conf.h
	gcc -o word_of_mouth_player word_of_mouth_player.c

clean:
	rm password_whisperer vault word_of_mouth_player
