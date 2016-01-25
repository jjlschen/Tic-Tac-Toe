/*
Practice programming in winter vacation
Checked serveral times and found no bug so far
Done on 2016/1/19
By J.Shine.Yamaha.Chenkerymiowstone
*/

# include <stdio.h>
# include <conio.h>
#include <stdlib.h>
#include <time.h>

# define O	1
# define X	(-1)
# define center_index	4
# define Esc		(-1)
# define Restart	(-2)
# define Yes		(-3)
# define No			(-4)

# define num2str(x)	((x)+48)
# define str2num(x)	((x)-48)
# define indx2num(x) ((x)+1)
# define num2indx(x) ((x)-1)

void initialize(int *AI_phase, int *end_flag, int *user_record, int *AI_record, int *space);
int inform(int *first_flag);
int AI_round(int first_flag, int round_num, int *AI_phase, int *end_flag, int *start_type, int *space, int *AI_record, int *user_record);
int AI_attack_phase(int round_num, int *space, int *AI_record);
int AI_defense_phase(int round_num, int *space, int *AI_record);
int check_horizontal(int acc, int O_or_X, int round_num, int *space, int *AI_record);
int check_vertical(int acc, int O_or_X, int round_num, int *space, int *AI_record);
int check_diagonal(int acc, int O_or_X, int round_num, int *space, int *AI_record);
int AI_free_phase(int start_type, int round_num, int *space, int *AI_record, int *user_record);
int AI_free_type1(int round_num, int *space, int *AI_record, int *user_record);
int AI_free_type2(int round_num, int *space, int *AI_record, int *user_record);
int AI_free_type3(int round_num, int *space, int *AI_record);
int AI_free_type4(int round_num, int *space, int *AI_record, int *user_record);
int AI_free_attack(int round_num, int *space, int *AI_record);
void AI_free_anomous(int round_num, int *space, int *AI_record);
int update_space(int *space);
void easyplot();
int user_round(int round_num, int *end_flag, int *space, int *user_record);
int easyscanf();
int stop_game(int command);
int end_of_game(int AI_win_flag);

char c_space[9], i_space[9];
int corner[4] = {0, 2, 6, 8};
int cross[2][4]  = {{ 1, 5, 3, 7},
					{ 3, 1, 7, 5}};

int main()
{
	int first_flag, AI_phase, round_num, start_type, end_flag;
	int space[9], user_record[9], AI_record[9];
	int AI_win_flag, command;
	
	while(1) {
		initialize(&AI_phase, &end_flag, user_record, AI_record, space);
		command = inform(&first_flag);
		if(command) break;
		for(round_num=0; ; round_num++)
		{
			AI_win_flag = AI_round(first_flag, round_num, &AI_phase, &end_flag, &start_type, space, AI_record, user_record);
			if(AI_win_flag || end_flag) break;
			
			while(1) {
				command = user_round(round_num, &end_flag, space, user_record);
				if(!command) break;
				else if( stop_game(command) ) break;
			}
			if(end_flag) break;
			else if(command) break;
		}
		if(command==Restart) continue;
		else if(command==Esc) break;
		if( end_of_game(AI_win_flag) ) break;
	}
	return 0;
}


void initialize(int *AI_phase, int *end_flag, int *user_record, int *AI_record, int *space)
{
	*AI_phase = 0;
	*end_flag = 0;
	system("mode CON: COLS=65 LINES=16");
	int i;
	for(i=0; i<9; i++) {
		space[i] = 0;
		user_record[i] = 0;
		AI_record[i] = 0; 
	}
	update_space(space);
	srand(time(NULL));
}


int inform(int *first_flag)
{
	int i, command, valid = 0;
	printf("\n\n");
	printf("\n This is a little game \"Tic Tac Toe\"!\n");
	printf("\n Want first? (Y/N)   (press \"Esc\" to exit)\n");
	while(!valid)
	{
		command = easyscanf();
		if (command==Yes) {
			_sleep(300);
			*first_flag = 1;
			valid = 1;
		}
		else if(command==No) {
			_sleep(300);
			*first_flag = 0;
			valid = 1;
		}
		else if(command==Esc) return Esc;
		else {
			valid = 0;
			if(command==Restart) {
				_sleep(300);
				printf("\b \b\b\b\b\b\b\b");
			}
		}
	}
	easyplot();
	printf("\n");
	return 0;
}


int AI_round(int first_flag, int round_num, int *AI_phase, int *end_flag, int *start_type, int *space, int *AI_record, int *user_record)
{
	int i, u0;
	_sleep(500);
	switch(*AI_phase)
	{
		case 0: {
			// check first_flag
			if(!first_flag) {
				*start_type = 1;
				space[center_index] = X;
				AI_record[0] = 5;
				*AI_phase = 2;
			}
			else *AI_phase = 1;
			break;
		}
		case 1: {
			// check center of space
			*AI_phase = 2;
			u0 = num2indx(user_record[0]);
			if(u0==center_index) *start_type = 2;
			else {
				*start_type = (u0==0 || u0 ==2 || u0==6 || u0==8) ? 3 : 4;
				space[center_index] = X;
				AI_record[round_num] = indx2num(center_index);
				break;
			}
		}
		case 2: {
			if( AI_attack_phase(round_num, space, AI_record) ) {
				*end_flag = update_space(space);
				easyplot();
				return 1;
			}
			if( AI_defense_phase(round_num, space, AI_record) ) break;
			AI_free_phase(*start_type, round_num, space, AI_record, user_record);
			break;
		}
	}
	*end_flag = update_space(space);
	easyplot();
	return 0;
}


int AI_attack_phase(int round_num, int *space, int *AI_record)
{
	// check horizontal direction
	if( check_horizontal(2, X, round_num, space, AI_record) ) return 1;
	// check vertical direction
	if( check_vertical(2, X, round_num, space, AI_record) )   return 1;
	// check oblique direction
	if( check_diagonal(2, X, round_num, space, AI_record) )   return 1;
	// no attack
	return 0;
}


int AI_defense_phase(int round_num, int *space, int *AI_record)
{
	// check horizontal direction
	if( check_horizontal(2, O, round_num, space, AI_record) ) return 1;
	// check vertical direction
	if( check_vertical(2, O, round_num, space, AI_record) )   return 1;
	// check oblique direction
	if( check_diagonal(2, O, round_num, space, AI_record) )   return 1;
	// no defense
	return 0;
}


int check_horizontal(int acc, int mark, int round_num, int *space, int *AI_record)
{
	int i, j, index, chance, flaw;
	for(i=0; i<3; i++) {
		chance = 0;
		flaw = 0;
		for(j=0; j<3; j++) {
			index = 3*i+j;
			chance += space[index];
			if(!space[index]) flaw = indx2num(index);
		}
		if(chance==acc*mark && flaw!=0) {
			index = num2indx(flaw);
			space[index] = X;
			AI_record[round_num] = flaw;
			return 1;
		}
	}
	return 0;
}


int check_vertical(int acc, int mark, int round_num, int *space, int *AI_record)
{
	int i, j, index, chance, flaw;
	for(i=0; i<3; i++) {
		chance = 0;
		flaw = 0;
		for(j=0; j<3; j++) {
			index = i+3*j;
			chance += space[index];
			if(!space[index]) flaw = indx2num(index);
		}
		if(chance==acc*mark && flaw!=0) {
			index = num2indx(flaw);
			space[index] = X;
			AI_record[round_num] = flaw;
			return 1;
		}
	}
	return 0;
}


int check_diagonal(int acc, int mark, int round_num, int *space, int *AI_record)
{
	int i, chance, flaw, index;
	chance = 0;
	flaw = 0;
	for(i=0; i<3; i++) {
		index = 4*i;
		chance += space[index];
		if(!space[index]) flaw = indx2num(index);
	}
	if(chance==acc*mark && flaw!=0) {
		index = num2indx(flaw);
		space[index] = X;
		AI_record[round_num] = flaw;
		return 1;
	}
	// check another direction
	chance = 0;
	flaw = 0;
	for(i=0; i<3; i++) {
		index = 2*(i+1);
		chance += space[index];
		if(!space[index]) flaw = indx2num(index);
	}
	if(chance==acc*mark && flaw!=0) {
		index = num2indx(flaw);
		space[index] = X;
		AI_record[round_num] = flaw;
		return 1;
	}
	return 0;
}


int AI_free_phase(int start_type, int round_num, int *space, int *AI_record, int *user_record)
{
	int i, u0, u1, a0, a1, inv_corner, index;
	switch(start_type)
	{
		case 1: {
			if( AI_free_type1(round_num, space, AI_record, user_record) ) return 1;
			else break;
		}
		case 2: {
			if( AI_free_type2(round_num, space, AI_record, user_record) ) return 1;
			else break;
		}
		case 3: {
			if( AI_free_type3(round_num, space, AI_record) ) return 1;
			break;
		}
		case 4: {
			if( AI_free_type4(round_num, space, AI_record, user_record) ) return 1;
			break;
		}
	}
	if( AI_free_attack(round_num, space, AI_record) ) return 1;
	AI_free_anomous(round_num, space, AI_record);
	return 0;
}


int AI_free_type1(int round_num, int *space, int *AI_record, int *user_record) 
{
	int i, u0, a1, inv_corner, index;
	switch(round_num)
	{
		case 1: {
			u0 = num2indx(user_record[0]);
			for(i=0; i<4; i++) {
				if(u0==corner[i] || u0==cross[0][i]) {
					inv_corner = corner[3-i];
					space[inv_corner] = X;
					AI_record[1] = indx2num(inv_corner);
					return 1;
				}
			}
			break;
		}
		case 2: {
			a1 = num2indx( AI_record[1]);
			for(i=0; i<4; i++) {
				if(corner[i]==a1) {
					index = (!space[cross[0][i]]) ? 0 : 1;
					space[cross[index][i]] = X;
					AI_record[2] = indx2num(cross[index][i]);
					return 1;
				}
			}
			break;
		}
	}
	return 0;
}


int AI_free_type2(int round_num, int *space, int *AI_record, int *user_record)
{
	int i, a0, u1, inv_corner, index;
	switch(round_num)
	{
		case 1: {
			index = corner[rand()%4];
			space[index] = X;
			AI_record[1] = indx2num(index);
			return 1;
		}
		case 2: {
			a0 = num2indx(   AI_record[1]);
			u1 = num2indx( user_record[1]);
			for(i=0; i<4; i++) {
				inv_corner = corner[3-i];
				if(corner[i]==a0 && inv_corner==u1) {
					index = (i==1 || i==2) ? (3*(rand()%2)) : (1+(rand()%2));
					index = corner[index];
					space[index] = X;
					AI_record[2] = indx2num(index);
					return 1;
				}
			}
			break;
		}
	}
	return 0;
}


int AI_free_type3(int round_num, int *space, int *AI_record)
{
	int i, index;
	if(round_num==2) {
		do {
			index = cross[0][rand()%4];
		} while(space[index]);
		space[index] = X;
		AI_record[2] = indx2num(index);
		return 1;
	}
	return 0;
} 


int AI_free_type4(int round_num, int *space, int *AI_record, int *user_record)
{
	int i, j, index, u0, u1;
	if(round_num==2) {
		u0 = num2indx(user_record[0]);
		u1 = num2indx(user_record[1]);
		if(u0==1 || u0==7) {
			i = u0/3;
			j = u1%3;
		}
		else {
			i = u1/3;
			j = u0%3;
		}
		index = i*3+j;
		if(!space[index]) {
			space[index] = X;
			AI_record[2] = indx2num(index);
			return 1;
		}
	}
	return 0;
}


int AI_free_attack(int round_num, int *space, int *AI_record)
{
	// check horizontal direction
	if( check_horizontal(1, X, round_num, space, AI_record) ) return 1;
	// check vertical direction
	if( check_vertical(1, X, round_num, space, AI_record) )   return 1;
	// check oblique direction
	if( check_diagonal(1, X, round_num, space, AI_record) )   return 1;
	// no attack
	return 0;
}


void AI_free_anomous(int round_num, int *space, int *AI_record)
{
	int i, null_count = 0, rand_index;
	int null_index[9];
	for(i=0; i<9; i++) {
		if(!space[i]) {
			null_index[null_count] = i;
			null_count++;
		}
	}
	rand_index = null_index[rand()%null_count];
	space[rand_index] = X;
	AI_record[round_num] = indx2num(rand_index);
}


int update_space(int *space)
{
	int i, null_count = 0;
	for(i=0; i<9; i++) {
		if(space[i]==0) {
			null_count ++;
			i_space[i] = num2str(i+1);
			c_space[i] = ' ';
		}
		else {
			i_space[i] = ' ';
			c_space[i] = (space[i]==X) ? 'X' : 'O';
			}
	}
	if(!null_count) return 1;
	else return 0;
}


void easyplot()
{
	int i;
	system("mode CON: COLS=65 LINES=16");
	printf("\n\n\n\n");
	printf("\t %c | %c | %c  ", c_space[0], c_space[1], c_space[2]);
	printf("\t %c | %c | %c\n", i_space[0], i_space[1], i_space[2]);
	printf("\t---+---+---  ");
	printf("\t---+---+---\n");
	printf("\t %c | %c | %c  ", c_space[3], c_space[4], c_space[5]);
	printf("\t %c | %c | %c  ", i_space[3], i_space[4], i_space[5]);
	printf("    Press \"R\" to restart\n");
	printf("\t---+---+---  ");
	printf("\t---+---+---\n");
	printf("\t %c | %c | %c  ", c_space[6], c_space[7], c_space[8]);
	printf("\t %c | %c | %c  ", i_space[6], i_space[7], i_space[8]);
	printf("    Press \"Esc\" to exit\n");
	printf("\n\n\n");
}


int user_round(int round_num, int *end_flag, int *space, int *user_record)
{
	int i, grid_num, index, valid = 0;
	while(!valid)
	{
		printf(" Which grid to place? (key number 1 to 9 and press \"Enter\")\n");
		grid_num = easyscanf();
		if(grid_num==Esc || grid_num==Restart) return grid_num;
		index = num2indx(grid_num);
		easyplot();
		if(grid_num<1 || grid_num>9) {
			valid = 0;
		}
		else {
			valid = (space[index]==0);
		}
	}
	space[index] = O;
	user_record[round_num] = grid_num;
	*end_flag = update_space(space);
	easyplot();
	printf("\n");
	return 0;
}


int easyscanf()
{
	int limit = 20;
	char ch[limit+1] = {0};
	int i, j, num, trash_start = 0;
	printf(" >>>  ");
	for(i=0; ; i++) {
		// overflow
		if(i==limit) {
			i = 0;
			trash_start = 0;
			for(j=0; j<limit; j++) {
				ch[j] = 0;
				printf("\b \b");
			}
		}
		ch[i] = getche();
		// press Esc 
		if(ch[i]==27) return Esc;
		// press enter
		else if(ch[i]==13) {
			if(i==0) {
				printf(" >>>  ");
				i = -1;
				continue;
			}
			else if (trash_start) {
				for(j=0; j<i+6; j++) printf(" ");
				for(j=0; j<i  ; j++) {
					printf("\b");
					ch[j] = 0;
				}
				printf("\b\b\b\b\b>>>  ");
				ch[i] = 0;
				trash_start = 0;
				i = -1;
				continue;
			}
			else {
				for(j=0; j<i+6; j++) printf(" ");
				for(j=0; j<i+6; j++) printf("\b");
				break;
			}
		}
		// press backspace
		else if(ch[i]==8) {
			printf(" ");
			ch[i] = 0;
			if(i==trash_start) trash_start = 0;
			if(i==0) i = -1;
			else {
				printf("\b");
				ch[i-1] = 0;
				i -= 2;
			}
			continue;
		}
		// start typeing
		num = str2num(ch[i]);
		if(i==0) {
			if(ch[0]=='r' || ch[0]=='R') return Restart;
			else if(ch[0]=='y' || ch[0]=='Y') return Yes;
			else if(ch[0]=='n' || ch[0]=='N') return No;
			if(num<1 || num>9) trash_start = 1;
		}
		else if(!trash_start){
			if(num<0 || num>9) trash_start = i+1;
		}
	}
	// output the number
	num = 0;
	for(j=0; j<i; j++) {
		num = num*10 + str2num(ch[j]);
	}
	return num;
}


int stop_game(int command)
{
	easyplot();
	if(command==Esc) printf(" Want exit game? (Y/N)\n");
	else if(command==Restart) printf(" Want restart game? (Y/N)\n");
	else return 0;
	while(1) {
		command = easyscanf();
		switch(command) {
			case Yes:
				_sleep(300);
				return 1;
			case No: {
				_sleep(300);
				easyplot();
				return 0;
			}
		}
		if(command== Esc || command==Restart) {
			_sleep(300);
			printf("\b \b\b\b\b\b\b\b");
		}
	}
}


int end_of_game(int AI_win_flag)
{
	int valid, command;
	easyplot();
	if(AI_win_flag) printf(" You Lose (=w=)...");
	else printf(" Draw (=w=)...");
	printf("   (press \"R\" or \"Esc\")\n");
	while(1) {
		command = easyscanf();
		switch(command) {
			case Esc:
				return 1;
			case Restart: {
				_sleep(300);
				return 0;
			}
		}
		if(command== Yes || command==No) {
			_sleep(300);
			printf("\b \b\b\b\b\b\b\b");
		}
	}
}
