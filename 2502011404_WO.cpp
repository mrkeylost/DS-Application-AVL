#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Data{
	
	char name[30];
	int fee;
	char location[50];
	int crew;
	char website[50];
	
	Data *right = NULL;
	Data *left = NULL;
	
	int height;
};

Data *createData(char name[], int fee, char location[], int crew, char website[]){
	Data *newData = (Data*)malloc(sizeof(Data));
	
	strcpy(newData->name, name);
	newData->fee = fee;
	strcpy(newData->location, location);
	newData->crew = crew;
	strcpy(newData->website, website);
	
	newData->right = NULL;
	newData->left = NULL;
	
	newData->height = 1;
	
	return newData;
}

int max(int a, int b){
	if(a > b){
		return a;
	}
	
	return b;
}

int get_height(Data *node){
	if(node == NULL){
		return 0;
	}
	
	return node->height;
}

int get_balance(Data *node){
	if(node == NULL){
		return 0;
	}
	
	return get_height(node->left) - get_height(node->right);
}

Data *right_rotate(Data *node){
	Data *x = node->left;
	Data *sub_x = x->right;
	
	x->right = node;
	node->left = sub_x;
	
	node->height = 1 + max(get_height(node->left), get_height(node->right));
	x->height = 1 + max(get_height(x->left), get_height(node->right));
	
	return x;
}

Data *left_rotate(Data *node){
	Data *x = node->right;
	Data *sub_x = x->left;
	
	x->left = node;
	node->right = sub_x;
	
	node->height = 1 + max(get_height(node->left), get_height(node->right));
	x->height = 1 + max(get_height(x->left), get_height(node->right));
	
	return x;
}

Data *insert(Data *node, char name[], int fee, char location[], int crew, char website[]){
	if(node == NULL){
		return (createData(name, fee, location, crew, website));
	}
	
	if(strcmp(name, node->name) < 0){
		node->left = insert(node->left, name, fee, location, crew, website);
	} else if(strcmp(name, node->name) > 0){
		node->right = insert(node->right, name, fee, location, crew, website);
	} else {
		return node;
	}
	
	node->height = 1 + max(get_height(node->left), get_height(node->right));
	
	int balance = get_balance(node);
	
	if(balance > 1 && strcmp(name, node->left->name) < 0){
		return right_rotate(node);
	} else if(balance > 1 && strcmp(name, node->left->name) > 0){
		node->left = left_rotate(node->left);
		return right_rotate(node);
	} else if(balance < -1 && strcmp(name, node->right->name) > 0){
		return left_rotate(node);
	} else if(balance < -1 && strcmp(name, node->right->name) < 0){
		node->right = right_rotate(node->right);
		return left_rotate(node);
	}
	
	return node;
}

Data *find(Data *node, char name[]){
	if(node == NULL){
		return NULL;
	}
	
	if(strcmp(name, node->name) < 0){
		return find(node->left, name);
	} else if(strcmp(name, node->name) > 0){
		return find(node->right, name);
	}
	
	return node;
}

Data *get_pred(Data *node){
	Data *curr = node->left;
	
	while(curr->right){
		curr = curr->right;
	}
	
	return curr;
}

Data *deletes(Data *node, char name[]){
	if(node == NULL){
		return NULL;
	}
	
	if(strcmp(name, node->name) < 0){
		node->left = deletes(node->left, name);
	} else if(strcmp(name, node->name) > 0){
		node->right = deletes(node->right, name);
	} else {
		
		Data *temp = NULL;
		
		if(node->left == NULL || node->right == NULL){
			if(node->left){
				temp = node->left;
			} else {
				temp = node->right;
			}
			
			free(node);
			node = temp;
			
			if(node == NULL){
				return NULL;
			}
		} else {
			
			Data *pred = get_pred(node);
			strcpy(node->name, pred->name);
			node->fee = pred->fee;
			strcpy(node->location, pred->location);
			node->crew = pred->crew;
			strcpy(node->website, pred->website);
			
			node->left = deletes(node->left, pred->name);
		}
	}
	
	if(node == NULL){
		return node;
	}
	
	node->height = 1 + max(get_height(node->left), get_height(node->right));
	
	int balance = get_balance(node);
	
	if(balance > 1 && get_balance(node->left) >= 0){
		return right_rotate(node);
	} else if(balance > 1 &&  get_balance(node->left) < 0){
		node->left = left_rotate(node->left);
		return right_rotate(node);
	} else if(balance < -1 && get_balance(node->right) <= 0){
		return left_rotate(node);
	} else if(balance < -1 && get_balance(node->right) > 0){
		node->right = right_rotate(node->right);
		return left_rotate(node);
	}
	
	return node;
}

void deleteAll(Data *node){
	if(node != NULL){
		deleteAll(node->right);
		deleteAll(node->left);
		node->left = node->right = NULL;
		free(node);
		node = NULL;
	}
}

void pre_order(Data *node){
	if(node != NULL){
		printf("%-20s | %-15d | %-15s | %-15d | %-15s\n", node->name, node->fee, node->location, node->crew, node->website);
		pre_order(node->left);
		pre_order(node->right);
	} else {
		return;
	}
}

void in_order(Data *node){
	if(node != NULL){
		in_order(node->left);
		printf("%-20s | %-15d | %-15s | %-15d | %-15s\n", node->name, node->fee, node->location, node->crew, node->website);
		in_order(node->right);
	} else {
		return;
	}
}

void post_order(Data *node){
	if(node != NULL){
		post_order(node->left);
		post_order(node->right);
		printf("%-20s | %-15d | %-15s | %-15d | %-15s\n", node->name, node->fee, node->location, node->crew, node->website);
	} else {
		return;
	}
}

void menu(){
	
	Data *root = NULL;
	
	menu:
	int input;
	
	do{
		printf("WO Wedding Organizer\n");
		printf("1. Add Available Wedding Organizer\n");
		printf("2. Choose Available Wedding Organizer\n");
		printf("3. Book All Available Wedding Organizer\n");
		printf("4. View Available Wedding Organizer\n");
		printf("5. Exit\n>> ");
		scanf("%d", &input);
		system("cls");
	} while(input < 1 || input > 5);
	
	system("cls");
	
	switch(input){
		
		case 1:
						
			char name[30];
			int fee;
			char location[50];
			int crew;
			char website[50];
			int nameLen, locLen, webLen, space, start, end;
			
			do{
				fflush(stdin);
				printf("Wedding Organizer Name [3-25]: ");
				scanf("%[^\n]", name);
				nameLen = strlen(name);
			} while(nameLen < 3 || nameLen > 25);
			
			do{
				fflush(stdin);
				printf("Wedding Organizer Fee[1,000,000 - 50,000,000]: ");
				scanf("%d", &fee);
			} while(fee < 1000000 || fee > 50000000);
			
			do{
				fflush(stdin);
				printf("Wedding Organizer Location [4 - 30 Character]: ");
				scanf("%[^\n]", location);
				locLen = strlen(location);
			} while(locLen < 4 || locLen > 30);
			
			do{
				fflush(stdin);
				printf("Wedding Organizer Total Crew [2-2000]: ");
				scanf("%d", &crew);
			} while(crew < 2 || crew > 2000);
			
			do{
				fflush(stdin);
				printf("Wedding Organizer Website [13-25]: ");
				scanf("%s", website);
				webLen = strlen(website);
				
				for(int i = 0;i<webLen;i++){
					if(website[i] == ' '){
						space++;
					}
				}
				
			} while((webLen < 13 || webLen > 25) || space > 0 || website[0] != 'w' || website[1] != 'w' || website[2] != 'w' || website[webLen-1] != 'd' || website[webLen-2] != 'i' || website[webLen-3] != '.' || website[webLen-4] != 'o' || website[webLen-5] != 'w' || website[webLen-6] != '.');
			
			char confirm;
			int conValid;
			
			do{
				fflush(stdin);
				printf("Confirm Inser [y/n]: ");
				scanf("%c", &confirm);
				
				if(confirm == 'y'){
				 	conValid = 1;
				 	
				 	printf("insert Success\n");
				 	
				 	root = insert(root, name, fee, location, crew, website);
				} else if(confirm == 'n'){
					conValid = 2;
					
					printf("Cancel Input\n");
				}
			}while (conValid < 1 || conValid > 2);
			
			puts("");
			printf("press enter to continue...");
			getchar(); getchar();
			system("cls");
			goto menu;
			
			break;
			
		case 2:
			char call[50];
			char concall;
			int callValid;
			
			if(root == NULL){
				printf("All wedding organizer already booked\n");
			} else {
				in_order(root);
				
				fflush(stdin);
				printf("Input Organizer name: ");
				scanf("%[^\n]", call);
				
				Data *result = find(root, call);
				
				if(result == NULL){
					printf("Organizer not found\n");
				} else {
					
					do{
						fflush(stdin);
						printf("are you sure?\n");
						printf("Confirm [y/n]: ");
						scanf("%c", &concall);
						
						if(concall == 'y'){
						 	callValid = 1;
						 	
						 	printf("reservation for %s success\n", call);
						 	
						 	root = deletes(root, result->name);
						} else if(concall == 'n'){
							callValid = 2;
							
							printf("Cancel Input\n");
						}
					}while (callValid < 1 || callValid > 2);
				}
			}
			
			puts("");
			printf("press enter to continue...");
			getchar(); getchar();
			system("cls");
			goto menu;
			
			
			break;
			
		case 3:
			
			if(root == NULL){
				printf("All Wo has been booked");
			} else{
				deleteAll(root);
			}
			
			puts("");
			printf("press enter to continue...");
			getchar(); getchar();
			system("cls");
			goto menu;
			
			break;
			
			break;
			
		case 4:
			
			char order[15];
			int type;
			
			if(root == NULL){
				printf("There's no Wedding organizer\n");
			} else {
				
				do{
					fflush(stdin);
					printf("How do want to look the data:\n");
					printf("1. preorder\n");
					printf("2. inorder\n");
					printf("3. postorder\n");
					scanf("%d", &type);
					system("cls");
				} while(type < 1 || type > 3);
				
				system("cls");
				
				if(type == 1){
					pre_order(root);
				} else if(type == 2){
					in_order(root);
				} else if(type == 3){
					post_order(root);
				}
			}
			
			puts("");
			printf("press enter to continue...");
			getchar(); getchar();
			system("cls");
			goto menu;
			
			break;
			
		case 5:
			exit(0);
			break;
	}
}

int main(){
	
	menu();
	
	return 0;
}
