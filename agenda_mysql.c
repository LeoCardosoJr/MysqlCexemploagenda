#include <stdio.h>
#include <stdlib.h>
#include <string.h> // para uso de strcat
#include <mysql/mysql.h> // instalar apt-get install libmysqlclient-dev
//biblioteca para consertar codificação 
#include <locale.h>

int main(void){
	//Seta as varíaveis do MySQL
	#define HOST "127.0.0.1"
	#define USER "root"
	#define PASS ""
	#define DB "c"

	MYSQL conexao;
	MYSQL_RES *resp;
	MYSQL_ROW linhas;
	MYSQL_FIELD *campos;

    float nota_ano, nota_exame, *notas, nota_media;
    int op, qtd_notas, cont, j, conta, menu = 0;

    char nome[101] = "", email[101] = "", celular[21] = "", id[11] = "";
    char query_pesquisa[] = "SELECT * FROM agenda;";
    char query_deleta[]   = "DELETE FROM agenda WHERE id = '";
    char query_alt[]      = "UPDATE agenda SET nome='";
    char query_insere[]   = "INSERT INTO agenda (nome, email, celular) VALUES ('";
   // char query[]="SELECT * FROM agenda;";

// setar local, mas foi apenas necessário alterar a codificação   setlocale(LC_ALL, "portuguese");
    while(menu == 0){
		printf("Escolha uma das opções abaixo\n");
		printf("1 - Verificar aprovação do aluno\n");
		printf("2 - Média\n");
		printf("3 - Agenda telefonica MySQL\n");
		printf("4 - Sair\n");
		scanf("%d", &op);
		switch (op)
		{
		    case 1: // Diz se o aluno foi aprovado ou reprovado
		    	menu = 1;
		        printf("Escreva a média anual do aluno\n");
		        scanf("%f", &nota_ano);
		        if(nota_ano >= 7){
		                    printf("O aluno esta aprovado\n");
		        }else{
		                        printf("Escreva a nota do aluno no exame\n");
		                        scanf("%f", &nota_exame);
		            if(nota_exame >= 5){
		                        printf("O aluno foi aprovado no exame!\n");
		            }else{
		                        printf("O aluno foi reprovado\n");
		            }
		        }
	// escreve comando terminal      system("pause");
		    break;
		    case 2: // Calcula a média do aluno, independente da quantidade de notas
		    	menu = 1;
		        printf("Escreva a quantidade de notas que o aluno teve\n");
		        scanf("%i", &qtd_notas);
		        cont = 0;
				notas = (float *) malloc(qtd_notas * sizeof(float));
		        do
		        {
		            cont++;
		                        printf("Escreva a nota número %i\n", cont);
		                        scanf("%f", &notas[cont]);
		                        if(notas[cont] > 10){
		                        	printf("Nota alta demais. Tente novamente.\n\n");
		                        	cont--;
		                        }else if(notas[cont] < 0){
		                        	printf("Nota baixa demais. Tente novamente.\n\n");
		                        }
		        } while (qtd_notas > cont);
		        for (j = 1; j <= cont; j++) {
		                printf("Nota[%i] = %.2f\n", j, notas[j] );
		                nota_media = notas[j] + nota_media;
		        }
		        printf("A pontuação total do aluno foi: %.2f \n\n", nota_media);
		        printf("                        ----------\n");
		        printf("A média do aluno foi de:|| %.2f ||\n", nota_media / cont);
		        printf("                        ----------\n");
		        free(notas);
		    break;
		    case 3: // Agenda telefonica MySQL
		    	menu = 1;
		    	while(menu == 1){
		    	printf("Escolha uma das opções abaixo\n");
				printf("1 - Cadastrar contato\n");
				printf("2 - Editar contato\n");
				printf("3 - Excluir contato\n");
				printf("4 - Fazer consulta\n");
				printf("5 - Voltar ao menu principal\n");
				printf("6 - Sair\n");
				scanf("%d", &op);
					switch (op)
					{
						case 1: // Efetua o CADASTRO a ação e volta ao menu principal
						menu = 1;
						printf("Escreva o nome do contato\n");
		       			scanf(" %s", nome); // %s para string
		       			setbuf(stdin, NULL);

		       			printf("Escreva o email do contato\n");
		       			scanf(" %s", email);
		       			setbuf(stdin, NULL);

		       			printf("Escreva o telefone do contato\n");
		       			scanf(" %s", celular);

						// query_insere = "INSERT INTO `agenda`(`nome`, `email`, `celular`) VALUES (";
						//falta [value-1],[value-2],[value-3],[value-4]) para a inclusão
						strcat(nome, "', '");
						strcat(email, "', '");
						strcat(celular, "')");

						strcat(query_insere, nome);
						strcat(query_insere, email);
						strcat(query_insere, celular);

						mysql_init(&conexao);
						if (mysql_real_connect(&conexao,HOST,USER,PASS,DB,0,NULL,0))
						{
							//printf("conectado com sucesso!\n");

							if (mysql_query(&conexao, query_insere)) { // pesquisa no bd
								printf("Erro: %s\n",mysql_error(&conexao));
								mysql_close(&conexao);
							}else{
									printf("Contato cadastrado com sucesso\n\n");
							}

						}
						else
						{
							printf("Falha de conexao\n");
							printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
						}

						break;
						case 2: // EDITA CONTATO
							menu = 1; // Efetua a ação e volta ao menu principal

							printf("Escreva o id do contato que deseja excluir\n");
				   			scanf(" %s", id); // %s para string
				   			setbuf(stdin, NULL);

							// query_insere = "UPDATE agenda SET id='";
							//falta [value-1]',`nome`=[value-2],`email`=[value-3],`celular`=[value-4] WHERE id =
							strcat(nome, "', email='");
							strcat(email, "', celular='");
							strcat(celular, "' WHERE id ='");
							strcat(id, "' ");

							strcat(query_alt, nome);
							strcat(query_alt, email);
							strcat(query_alt, celular);
							strcat(query_alt, id);

							mysql_init(&conexao);
							if (mysql_real_connect(&conexao,HOST,USER,PASS,DB,0,NULL,0))
							{
								//printf("conectado com sucesso!\n");

								if (mysql_query(&conexao, query_alt)) { // pesquisa no bd
									printf("Erro: %s\n",mysql_error(&conexao));
									mysql_close(&conexao);
								}else{
									printf("Contato alterado com sucesso\n\n");
								}

							}
							else
							{
								printf("Falha de conexao\n");
								printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
							}

						break;
						case 3: // EXCLUI CONTATO
							menu = 1; // Efetua a ação e volta ao menu principal

							printf("Escreva o id do contato\n");
				   			scanf(" %s", id); // %s para string

							//  char query_deleta[]   = "DELETE FROM agenda WHERE id = '";
							// falta id'
							strcat(id, "'");

							strcat(query_deleta, id);

							mysql_init(&conexao);
							if (mysql_real_connect(&conexao,HOST,USER,PASS,DB,0,NULL,0))
							{
								//printf("conectado com sucesso!\n");

								if (mysql_query(&conexao, query_deleta)) { // pesquisa no bd
									printf("Erro: %s\n",mysql_error(&conexao));
									mysql_close(&conexao);
								}else{
									printf("Contato excluído com sucesso\n\n");
								}

							}
							else
							{
								printf("Falha de conexao\n");
								printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
							}
						break;
						case 4:
							menu = 1; // Efetua a ação e volta ao menu principal

							mysql_init(&conexao);
							if (mysql_real_connect(&conexao,HOST,USER,PASS,DB,0,NULL,0))
							{
								//printf("conectado com sucesso!\n");

								if (mysql_query(&conexao,query_pesquisa)) // pesquisa no bd
										 printf("Erro: %s\n",mysql_error(&conexao));
									  else
									  {
										 resp = mysql_store_result(&conexao);//recebe a consulta
										if (resp) //se houver consulta
										{
										   //passa os dados dos campos para a variável campos
										   //escreve na tela os nomes dos campos dando
										   //um tab somente
										   campos = mysql_fetch_fields(resp);
										   for (conta=0;conta<mysql_num_fields(resp);conta++) {
											  printf("%s",(campos[conta]).name);
											  if (mysql_num_fields(resp)>1)
												  printf("\t");
											  }

											  printf("\n");

											  //enquanto retonrnar registros, conta até o
											  //número de colunas que a tabela tem e escreve na
											  //tela com um tab, depois pula a linha e tenta
											  //pegar outro registro
											  while ((linhas=mysql_fetch_row(resp)) != NULL)
											  {
												 for (conta=0;conta<mysql_num_fields(resp);conta++)
													printf("%s\t",linhas[conta]);
												 	printf("\n");
											  }
										  }
										  mysql_free_result(resp);//limpa a variável do resultado: resp
										}
										mysql_close(&conexao);

							}
							else
							{
								printf("Falha de conexao\n");
								printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
							}
						break;
						case 5:
							menu = 0; // Volta ao menu principal
						break;
						case 6:
							menu = 2; // Sai do programa
						break;
						default:
							printf("\nEscolha incorreta, o programa voltará ao menu principal.\n\n");
							menu = 1; // Volta ao menu principal
					}
				}
				break;
				case 4:
					menu = 1;
				    exit(0);
				break;
				default:
					printf("\nEscolha incorreta\n\n");
			}
	}
	return 0;
}
