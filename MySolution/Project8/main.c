#include <stdio.h>
#include <stdlib.h>

int LSR_Gossip(int Gender)
{
	Gender = 1;
	printf("\n�̼��� : ������ �������� ���̾�...\n");
	return 0;
}

int Wife_Command(int* Gender)
{
	Gender[0] = 1;
	printf("\n������ : �� ���úη� ���̾�...\n");
	return 0;
}

int main()
{
	int* ES_Maum;
	ES_Maum = (int*)malloc(sizeof(int) * 2);

	/* [0] �����ϴ� ����
	 *     0: ����
	 *     1: ����
	 *     2: ��Ÿ
	 * [1] ���� ����
	 *     0: ����
	 *     1: ����
	 */
	ES_Maum[0] = 0;
	ES_Maum[1] = 1;

	//LSR_Gossip(ES_Maum[0]);
	Wife_Command(ES_Maum);


	if (ES_Maum[0] == 1)
	{
		printf("�״� ���̿���.\n");
	}
	else if (ES_Maum[0] == 0)
	{
		printf("�״� ������ ����Ѵ�\n");
	}
	else if (ES_Maum[0] == 0)
	{
		printf("�״�... �Ͼ�...\n");
	}

	if (ES_Maum[1] == 1)
	{
		printf("�״� ������̴�.\n");
	}
	else if (ES_Maum[1] == 0)
	{
		printf("�״� �ܷӴ�.\n");
	}

	free(ES_Maum);
	return 0;
}
