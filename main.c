//Nume: Serboi D. Florea-Dan
//Grupa si seria: 315CB
/*Precizare: piesele sunt declarate ca unsigned long long. Astfel, vor fi
reprezentate pe 64 biti pt a usura calculele cu ele si deplasarile pe harta*/
#include <stdio.h>
#include <math.h>
void afisare_harta(unsigned long long harta)
{
	unsigned long long mask = 1;
	long long index = 63;
	while(index >= 0)
	{
		mask = 1;
		if((harta & (mask << index)) != 0)
			printf("#");
		else
			printf(".");
		if(index % 8 == 0)
			printf("\n");
		index--;
	}
	printf("\n");
}
int coliziune(unsigned long long harta, unsigned long long piesa)
{
	if((harta & piesa) != 0)
		return 1;
	else
		return 0;
}
void transformare(unsigned long long harta,
				  unsigned long long *piesa, 
				  long long transf_linie,
				  long long linie)
{
	unsigned long long aux_piesa, deplasare_piesa, margine_sg, margine_dr;
	aux_piesa = (*piesa);
	aux_piesa = aux_piesa << (8 - linie) * 8;
	/*in aux_piesa este memorata piesa noastra saltata pana la linia curenta
	(atat cat incape, de exemplu, o piesa pe 2 linii nu va fi intreaga pe
	prima linie a hartii)*/
	deplasare_piesa = 0;
	/*in deplasare_piesa retinem cu cat trebuie deplasata piesa noastra	
	in stanga sau dreapta*/
	if(transf_linie < 0)
	{	
		margine_sg = 32896;
		margine_sg = margine_sg << (8 - linie) * 8;
		/*prin margine_sg construim o masca care sa ne blocheze o eventuala 
		deplasare a piesei in exteriorul hartii in stanga*/
		while(coliziune(harta, aux_piesa << 1) == 0 && transf_linie != 0 &&
		coliziune(margine_sg, aux_piesa) == 0)
		{
		aux_piesa = aux_piesa << 1;
		transf_linie++;
		deplasare_piesa++;
		}
		(*piesa) = (*piesa) << deplasare_piesa;
	}
	else
	{
		margine_dr = 257;
		margine_dr = margine_dr << (8 - linie) * 8;
		/*prin margine_dr construim o masca care sa ne blocheze o eventuala
		deplasare a piesei in exteriorul hartii in dreapta*/
		while(coliziune(harta, aux_piesa >> 1) == 0 && transf_linie != 0 &&
		coliziune(margine_dr, aux_piesa) == 0)
		{
		    aux_piesa = aux_piesa >> 1;
		    transf_linie--;
		    deplasare_piesa++;
		}
		(*piesa) = (*piesa) >> deplasare_piesa;
	}
}
void elim_linii_complete(unsigned long long *harta, long long *nr_linii_elim)
{
	unsigned long long linie_completa, p_de_sus, p_de_jos;
	long i;
	for(i = 0; i <= 7; i++)
	{
		linie_completa = 255;
		linie_completa = linie_completa << (i * 8);
		/*prin variabila linie_completa construim o masca cu 8 biti de 1
		consecutivi cu ajutorul careia verificam daca o linie
		din harta e completa*/
		if((linie_completa | *harta) == *harta && i == 0)
		{
			*harta = *harta >> 8;
			i--;
			(*nr_linii_elim)++;
		}
		/*caz separat pentru linia de jos, aici harta va fi deplasata in jos
		si ultima linie va disparea, harta e completata cu 8 noi biti de 0
		pe linia de sus*/
		else
			if((linie_completa | *harta) == *harta && i == 7)
			{
				*harta = *harta << 8;
				*harta = *harta >> 8;
				i--;
				(*nr_linii_elim)++;
			}
			/*caz separat pentru linia de sus, aici harta
			va fi deplasata sus si apoi in jos, pt a elimina linia de sus,
			completand-o cu 0*/
			else
				if((linie_completa | *harta) == *harta)
				{
				p_de_sus = *harta >> ((i+1) * 8);
				p_de_sus = p_de_sus << (i * 8);
				p_de_jos = *harta << ((8-i) * 8);
				p_de_jos = p_de_jos >> ((8-i) * 8);
				*harta = (p_de_sus | p_de_jos);
				i--;
				(*nr_linii_elim)++;
				}
				/*aici eliminam celelate linii complete, construind harta prin
				reuniunea partii de deasupra liniei curente si partii de
				dedesubtul ei*/				
	}
}
long numar_zerouri(unsigned long long harta)
{
	unsigned long long mask;
	long long index = 63, zerouri = 0;
	while(index >= 0)
	{
		mask = 1;
		if((harta & (mask << index)) == 0)
			zerouri++;
		index--;
	}
	return zerouri;
}
int main()
{
	float formula;
	unsigned long long  harta, piesa, linie, aux_piesa;
	long long transf_linie, aux_nr_l_elim, mutari, j,
	nr_linii_elim = 0, game_over = 0, ignor_transf = 0, piesa_setata = 0;
	/*variabilele game_over, ignor_transf, respectiv piesa_setata sunt
	responsabile de valoarea de adevar a urmatoarelor propozitii:
	"jocul s-a terminat", "se ignora ulterioarele transformari", respectiv
	"piesa a fost setata de harta"*/
	scanf("%llu%llu", &harta, &mutari);
	if(mutari == 0)
		afisare_harta(harta);
	else
	{
		afisare_harta(harta);
		for(j = 1; j <= mutari; j++)
		{
			scanf("%llu", &piesa);
			ignor_transf = 0;
			piesa_setata = 0;
			for(linie = 1; linie <= 8; linie++)
			{
				scanf("%lld", &transf_linie);
				aux_piesa = piesa;
				aux_piesa = aux_piesa << (8 - linie) * 8;
				if(game_over == 0 && piesa_setata == 0 && ignor_transf == 0)
				{
					if(coliziune(harta, aux_piesa) == 1 && linie == 1)
					{
						game_over = 1;
						afisare_harta(harta);
					}
					/*daca o piesa nu are loc pe prima linie a hartii,
					jocul s-a terminat*/
					if(coliziune(harta, aux_piesa) == 1 && linie == 2 && 
					piesa > 255)
					{
						game_over = 1;
						harta = (harta | (aux_piesa << 8));
						afisare_harta(harta);
					}
					/*daca o piesa pe 2 linii nu are loc pe a doua linie
					a hartii, jocul s-a terminat*/
					if(coliziune(harta, aux_piesa) == 1)
					{
						harta = harta | (aux_piesa << 8);
						aux_nr_l_elim = nr_linii_elim;
						elim_linii_complete(&harta, &nr_linii_elim);
						if(aux_nr_l_elim != nr_linii_elim)
							afisare_harta(harta);
						ignor_transf = 1;
						piesa_setata = 1;
					}
					/*daca la deplasarea piesei in jos, avem o coliziune,
					pe harta se va seta piesa in pozitia pe care o avea
					pe linia anterioara; se vor elimina eventualele linii
					complete si se va afisa harta dupa eliminarea liniilor
					(daca o linie a fost eliminata, numarul de linii
					eliminate dinainte de operatie difera de cel de dupa
					efectuarea operatiei), si astfel se vor ignora
					urmatoarele transformari*/ 
					if(linie == 8 && coliziune(harta, aux_piesa) == 0)
					{
						transformare(harta, &piesa, transf_linie, linie);
						harta = harta | piesa;
						afisare_harta(harta);
						aux_nr_l_elim = nr_linii_elim;
						elim_linii_complete(&harta, &nr_linii_elim);
						if(aux_nr_l_elim != nr_linii_elim)
						    afisare_harta(harta);
						ignor_transf = 1;
					}
					/*aici tratam separat cazul cand piesa a ajuns pe linia
					de jos si nu exista vreo coliziune*/
					if(ignor_transf == 0)
					{
						transformare(harta, &piesa, transf_linie, linie);
						aux_piesa = piesa;
						aux_piesa = aux_piesa << (8 - linie) * 8;
						afisare_harta(harta | aux_piesa);
					}
					/*efectuam transformarea pt piesa (daca e posibil)
					si afisam harta de dupa transformare, folosind variabila
					aux_piesa pt "a salta" piesa la linia curenta*/
				}
			}
		}
	}
	printf("GAME OVER!\n");
	formula = sqrt(numar_zerouri(harta)) + pow(1.25, nr_linii_elim);
	printf("Score:%.2f", formula);
	return 0;
}