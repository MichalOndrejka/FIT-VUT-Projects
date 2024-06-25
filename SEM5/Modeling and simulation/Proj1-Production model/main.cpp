/*
 * Predmet: IMS (Modelovanie a simulácie)
 * Téma: Modelovanie a simulácia kľukového hriadeľa
 * Autori: Michal Ondrejka (xondre15), Dominik Trúchly (xtruch01)
 * Dátum: 8.12.2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <simlib.h>
#include <stdbool.h>
#include <iostream>


#define POCET_CNC_STROJOV 9
#define POCET_FREZOVACICH_STROJOV 5
#define POCET_PRETAHOVACICH_STROJOV 4
#define POCET_BRUSNYCH_STROJOV 5
#define POCET_MERACICH_STROJOV 2
#define POCET_DOLADOVACICH_STROJOV 4



#define RS_CHANCE 0.00011415525 // Šanca Ring Srew chyby
#define US_CHANCE 0.00001331811 // Šanca Unbalanced a Set Up chyby
#define P_CHANCE 0.00002283105 // Šanca Pins chyby
#define BD_CHANCE 0.00001902587 // Šanca Broken Drill chyby
#define H_CHANCE 0.00002853881 // Šanca Hub chyby
#define M_CHANCE 0.00005707762 // Šanca Mains chyby


#define SEC  * (1.0/60.0)
#define HOD * 60
#define DEN * 24 * 60

enum StrojEnum {
    CNC_STROJ_ENUM,
    FREZOVACI_STROJ_ENUM,
    PRETAHOVACI_STROJ_ENUM,
	BRUSNY_STROJ_ENUM,
	MERACI_STROJ_ENUM,
	DOLADOVACI_STROJ_ENUM,
};


unsigned int pocet_vyrobkov_vo_vyrobe = 0;
unsigned int pocet_hotovych_vyrobkov = 0;
unsigned int pocet_nepodarenych_vyrobkov = 0;

unsigned int pocet_udrzieb = 0;

unsigned int pocet_ring_screw_chyb = 0;
unsigned int pocet_unbalanced_chyb = 0;
unsigned int pocet_broken_drill_chyb = 0;
unsigned int pocet_hub_chyb = 0;
unsigned int pocet_mains_chyb = 0;
unsigned int pocet_set_up_chyb = 0;
unsigned int pocet_pins_chyb = 0;

Store cnc_stroj("CNC stroj", POCET_CNC_STROJOV); // Ring screw, Unbalanced, Broken Drill, Hub, Mains
Store frezovaci_stroj("Frezovaci stroj", POCET_FREZOVACICH_STROJOV); // Ring screw, Unbalanced, Pins, Hub, Mains
Store pretahovaci_stroj("Pretahovaci stroj", POCET_PRETAHOVACICH_STROJOV); // Ring screw, Unbalanced, Hub, Mains
Store brusny_stroj("Brusny stroj", POCET_BRUSNYCH_STROJOV); // Ring screw, Unbalanced, Hub, Mains
Store meraci_stroj("Meraci stroj", POCET_MERACICH_STROJOV); // Ring screw, Unbalanced, Hub, Mains
Store doladovaci_stroj("Doladovaci stroj", POCET_DOLADOVACICH_STROJOV); // Ring screw, Unbalanced, Hub, Mains

class Vyroba : public Process { // Enter s max hodnotou Store nejde
	void Behavior() {
		Priority = 100;
		int pocet_strojov = (POCET_CNC_STROJOV + POCET_BRUSNYCH_STROJOV + POCET_FREZOVACICH_STROJOV 
			+ POCET_MERACICH_STROJOV + POCET_DOLADOVACICH_STROJOV + POCET_PRETAHOVACICH_STROJOV)/6;
		while (true) {
			Wait(10/pocet_strojov HOD);
			Enter(cnc_stroj, 1);
			Enter(frezovaci_stroj, 1);
			Enter(pretahovaci_stroj, 1);
			Enter(brusny_stroj, 1);
			Enter(meraci_stroj, 1);
			Enter(doladovaci_stroj, 1);

			Wait(2/pocet_strojov HOD);
			Leave(cnc_stroj, 1);
			Leave(frezovaci_stroj, 1);
			Leave(pretahovaci_stroj, 1);
			Leave(brusny_stroj, 1);
			Leave(meraci_stroj, 1);
			Leave(doladovaci_stroj, 1);

			pocet_udrzieb++;;
		}
	}
};

class Vyrobok : public Process {
private:
	StrojEnum GetStrojEnum(Store &stroj) {
		if (stroj == cnc_stroj) return CNC_STROJ_ENUM;
		else if (stroj == frezovaci_stroj) return FREZOVACI_STROJ_ENUM;
		else if (stroj == pretahovaci_stroj) return PRETAHOVACI_STROJ_ENUM;
		else if (stroj == brusny_stroj) return BRUSNY_STROJ_ENUM;
		else if (stroj == meraci_stroj) return MERACI_STROJ_ENUM;
		else return DOLADOVACI_STROJ_ENUM;
	}

	void handleRingScrewFailure(Store &stroj) {
		Priority = 101;
		pocet_ring_screw_chyb++;
		Wait(Exponential(4 HOD));
		Leave(stroj, 1);
		pocet_vyrobkov_vo_vyrobe--;
		pocet_nepodarenych_vyrobkov++;
		Cancel();
	}

	void handleUnbalancedFailure(Store &stroj) {
		Priority = 101;
		pocet_unbalanced_chyb++;
		Wait(Exponential(1 HOD));
		Leave(stroj, 1);
		pocet_vyrobkov_vo_vyrobe--;
		pocet_nepodarenych_vyrobkov++;
		Cancel();
	}

	void handleBrokenDrillFailure(Store &stroj) {
		Priority = 101;
		pocet_broken_drill_chyb++;
		Wait(Exponential(5 HOD));
		Leave(stroj, 1);
		pocet_vyrobkov_vo_vyrobe--;
		pocet_nepodarenych_vyrobkov++;
		Cancel();
		
	}

	void handleHubFailure(Store &stroj) {
		Priority = 101;
		pocet_hub_chyb++;
		Wait(Exponential(2 HOD));
		Leave(stroj, 1);
		pocet_vyrobkov_vo_vyrobe--;
		pocet_nepodarenych_vyrobkov++;
		Cancel();
		
	}

	void handleMainsFailure(Store &stroj) {
		Priority = 101;
		pocet_mains_chyb++;
		Wait(Exponential(1 HOD));
		Leave(stroj, 1);
		pocet_vyrobkov_vo_vyrobe--;
		pocet_nepodarenych_vyrobkov++;
		Cancel();	
	}

	void handlePinsFailure(Store &stroj) {
		Priority = 101;
		pocet_pins_chyb++;
		Wait(Exponential(1 HOD));
		Leave(stroj, 1);
		pocet_vyrobkov_vo_vyrobe--;
		pocet_nepodarenych_vyrobkov++;
		Cancel();
	}

    void PerformOperation(Store &stroj, int waitTime) {
        Enter(stroj, 1);
        Wait(waitTime SEC);

		double r = Random();
		StrojEnum strojEnum = GetStrojEnum(stroj);
		switch(strojEnum) {
			case CNC_STROJ_ENUM: // Ring screw, Unbalanced, Broken Drill, Hub, Mains
				if (r < RS_CHANCE) handleRingScrewFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE) handleUnbalancedFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE + BD_CHANCE) handleBrokenDrillFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE + BD_CHANCE + H_CHANCE) handleHubFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE + BD_CHANCE + H_CHANCE + M_CHANCE) handleMainsFailure(stroj);
				break;
			case FREZOVACI_STROJ_ENUM: // Ring screw, Unbalanced, Pins, Hub, Mains
				if (r < RS_CHANCE) handleRingScrewFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE) handleUnbalancedFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE + P_CHANCE) handlePinsFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE + P_CHANCE + H_CHANCE) handleHubFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE + P_CHANCE + H_CHANCE + M_CHANCE) handleMainsFailure(stroj);
				break;
			default: // Ring screw, Unbalanced, Hub, Mains
				if (r < RS_CHANCE) handleRingScrewFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE) handleUnbalancedFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE + H_CHANCE) handleHubFailure(stroj);
				else if (r < RS_CHANCE + US_CHANCE + H_CHANCE + M_CHANCE) handleMainsFailure(stroj);
				break;
		}

        Leave(stroj, 1);
        Priority++;
    }

public:
    void Behavior() {
		Priority = 0;

        PerformOperation(cnc_stroj, 132);

        PerformOperation(frezovaci_stroj, 66);
        PerformOperation(frezovaci_stroj, 132);

        PerformOperation(pretahovaci_stroj, 198);

        PerformOperation(frezovaci_stroj, 66);

        PerformOperation(cnc_stroj, 132);
        PerformOperation(cnc_stroj, 132);

        PerformOperation(brusny_stroj, 66);
        PerformOperation(brusny_stroj, 66);

        PerformOperation(meraci_stroj, 66);

        PerformOperation(brusny_stroj, 132);

        PerformOperation(cnc_stroj, 66);

        PerformOperation(doladovaci_stroj, 198);

        pocet_vyrobkov_vo_vyrobe--;
        pocet_hotovych_vyrobkov++;
    }
};

class Generator : public Event {
	void Behavior() {
		pocet_vyrobkov_vo_vyrobe++;
		(new Vyrobok)->Activate();
		
		Activate(Time + 50 SEC);
	}
};

void print_stats(){
	printf("--- POČTY VÝROBKOV ---\n");
	printf("Počet hotových výrobkov = %u\n", pocet_hotovych_vyrobkov);
    printf("Počet výrobkov vo výrobe(nehotové kusy) = %u\n", pocet_vyrobkov_vo_vyrobe);
	printf("Počet nepodarených výrobkov = %u\n", pocet_nepodarenych_vyrobkov);

	printf("--- POČTY OBSLÚH ---\n");
	printf("Počet údržieb strojov = %d\n", pocet_udrzieb);

	printf("--- POČTY CHÝB ---\n");
	printf("Počet chýb kruhovych skrutiek = %u\n", pocet_ring_screw_chyb);
	printf("Počet chýb konfigurácie stroja = %u\n", pocet_unbalanced_chyb);
	printf("Počet chýb zlomeného vrtáka = %u\n", pocet_broken_drill_chyb);
	printf("Počet chýb strediska = %u\n", pocet_hub_chyb);
	printf("Počet chýb siete = %u\n", pocet_mains_chyb);
	printf("Počet chýb pinov = %u\n", pocet_pins_chyb);

	cnc_stroj.Output();
	frezovaci_stroj.Output();
	pretahovaci_stroj.Output();
	brusny_stroj.Output();
	meraci_stroj.Output();
	doladovaci_stroj.Output();
}

void run_simulation(int dlzka_simulacie){
	Init(0, dlzka_simulacie DEN);
	(new Vyroba)->Activate();
	(new Generator)->Activate();
	Run();
	print_stats();
}

int main(int argc, char **argv) {
	if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-h] number_of_days" << std::endl;
        return 1;
    }

    bool helpFlag = false;
    int dlzka_simulacie = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h") {
            helpFlag = true;
        } else {
            dlzka_simulacie = std::atoi(argv[i]);

            if (dlzka_simulacie <= 0) {
                std::cerr << "Invalid number of days to simulate. Please provide a positive integer." << std::endl;
                return 1;
            }
        }
    }

    if (helpFlag) {
        std::cout << "Usage: " << argv[0] << " [-h] number_of_days" << std::endl;
        std::cout << "\t-h: Display this help message" << std::endl;
        std::cout << "\tnumber_of_days: Duration of the simulation in days" << std::endl;
        return 0;
    }

	run_simulation(dlzka_simulacie);
	return 0;
}
