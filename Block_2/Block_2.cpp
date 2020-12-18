#include "SHA256.h"


struct user {
	
	std::string Name = "";
	std::string Public_Key = "";
	int Balance = 0;
};

user User[1000]; //kad nereiketu linkedlisto

struct Transaction {

	std::string ID = ""; //naudojama merkel medziui
	std::string Sender = "";
	std::string Receiver = "";
	int Sum = 0;

};

struct Block {
	std::string Version = "0.2v";
	std::string Merkel; //cia saugoma daug hashu suhasuota i viena
	int TimeStamp = 0;
	int Nonce = 0;
	int DifficultyTarget = 0;
	std::string Hash;
	std::string PreviousHash;
	std::vector<Transaction> AtrinktosT; //100 sugeneruotu transakciju
	Block* next = NULL;

};

Block* n; //node
Block* h = NULL; //header		//linked listas
Block* t; //temp

std::vector<Transaction> TransPool;		//visu transakciju poolas, globalus

void UserGenerator() {

	int ran;
	char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	for (int i = 0; i < 1000; i++){	// uzpildoma 1000 userio strukturu

		User[i].Name = "UserNum." + std::to_string(i + 1);	//kad 0 nario nebutu
		for (int y = 0; y < 32; y++){

			ran = std::round(0 + (double)rand() / RAND_MAX * 16); //nuo 0 iki 15 
			User[i].Public_Key = User[i].Public_Key + letters[ran];
		}
		ran = std::round(100 + (double)rand() / RAND_MAX * 999899); //valiuta nuo 100 iki 1.000.000
		User[i].Balance = ran;
	}
}

void TransactionGenerator() {	

	Transaction n;	//viena struktura
	
	for (int i = 0; i < 10000; i++){	//10.000 transakciju generuojama

		int ran;
		std::string u1, u2, u3;
		ran = std::round(0 + (double)rand() / RAND_MAX * 999); //paimamas vienas useris atsitiktinis kaip siuntejas
		n.Sender = User[ran].Public_Key;
		n.Sum = std::round(1 + (double)rand() / RAND_MAX * (User[ran].Balance - 1)); //sugeneruojama, kad kazkiek pinigu atidave nuo 1 iki max kiek tas asmuo turi
		for (int y = 0; y < 1; y++){

			ran = std::round(0 + (double)rand() / RAND_MAX * 999);
			if (User[ran].Public_Key == u1) y--;	//generuoja atsitiktini gaveja, jei toks pat, kaip siuntejas, generuoja naujai
		}
		n.Receiver = User[ran].Public_Key;

		//sender, receiver ir suma sudedami kaip stringai ir uzhashuojami
		n.ID = sha256(n.Sender + std::to_string(n.Sum) + n.Receiver);

		TransPool.push_back(n);		//sugeneruojama viena transakcija ir tada papushinama i poola
	}
	
}

bool Validation(Transaction tikrinimas) {	//tikinama transakcija pries padavima is poolo

	 //transakcija lieka - true, false - istrinti
	//jei A issiuncia daugiau nei turi

	int i = 0;
	while (tikrinimas.Sender != User[i].Public_Key) {
		i++;

	}

	if (User[i].Balance < tikrinimas.Sum) {
		return false;
	}

	if (tikrinimas.ID != sha256(tikrinimas.Sender + std::to_string(tikrinimas.Sum) + tikrinimas.Receiver)) {
		return false;
	}

	return true;

}

std::vector<Transaction> ChoosingTransactions() {	//pasirenkamos 100 transakciju

	int ran;
	std::vector<Transaction> Atrenkamos; //laikinos, kurios veliau paduodamos jau i bloko transakciju vektoriu
	//AtrinktosT // bloko atsirinktos 100
	//Transpool //visos kiek yra transakciju, 10000
		for (int i = 0; i < 100; i++) {

			ran = std::round(0 + (double)rand() / RAND_MAX * (TransPool.size() - 1));	//paimama atsitiktine transakcija
			if (Validation(TransPool.at(ran))) {	//patikrinama ar tinka
				Atrenkamos.push_back(TransPool.at(ran)); //paduodama is poolo jei tinka
			}
			else { i--; }	//jei netinka, iesko naujos ir istrina


			TransPool.erase(TransPool.begin() + ran);		//paduota transakcija istrinama is poolo

		};

		for (int j = 0; j < 100; j++) {	//transakciju grazinimas i poola, tam kad galetu persidengti
			TransPool.push_back(Atrenkamos.at(j));
		}
		return Atrenkamos; //jos keliauja i bloke paskirta transakciju vektoriu
}


std::string MerkelRoot(std::vector<Transaction> a)
{
	std::vector<std::string> hashes, hashes2; // pirmame vektoriuje visi hashai, antrame jau poromis suhashinti
	for (int i = 0; i < 100; i++)	//vaziuoja per visas 100 bloko transakciju
	{
		hashes.push_back(a.at(i).ID);
	}
	while (hashes.size() != 1)
	{
		for (int y = 0; y < hashes.size(); y++)
		{
			if (y + 1 != hashes.size()) {		//jei nepaskutinis hashas, hashuoja viena ir einanti po jo 
				hashes2.push_back(sha256(hashes.at(y) + hashes.at(y + 1)));
			}
			else {		//jei hashas paskutinis, hashuojasi pats su savimi
				hashes2.push_back(sha256(hashes.at(y) + hashes.at(y)));
			}
			y++;
		}
		hashes.clear();		//seni, dar nesujungti hashai dingsta, ateina jau karta sujungti hashai is antro vektoriaus, 
								//antrame vektoriuje vel vyksta sujungimas/hashinimas, perkelimas i pirma tol, kol lieka tik vienas hashas
		hashes = hashes2;
		hashes2.clear();
	}
	return hashes.at(0);
}

/* std::string BlockHash() {		sitoje versijoje nebenaudojama funkcija
	std::string Bhash, a1, a2; //bhash - galutinis bloko hashas
	int t = -1;
	while (t == -1){
		a1 = sha256(n->Merkel + std::to_string(n->TimeStamp)); //a1 ir a2 yra tarpiniai
		a2 = sha256(n->PreviousHash + std::to_string(n->Nonce));
		Bhash = sha256(a1 + a2);
	
		t = 0;
		for (int i = 0; i < n->DifficultyTarget; i++) // tikrina, ar sukurtas hashas atitinka nustatyta difficulty
		{
			if (Bhash.at(i) == '0') t++; //suskaiciuoja, kiek atitinka
		}
		if (t != n->DifficultyTarget) {	//cia patikrina, ar visi kiek reikia atitinka
			t = -1;
			n->Nonce = n->Nonce + 1; //jei neatitinka visi, keiciamas nonce, del jo keisis bendras hashas
		}
	}
	return Bhash;
} */


void TransactionDelete(std::vector<Transaction> trynimui) {	//paduodamos transakcijos naikinimui,
															//kadangi tik kandidatu transakcijos gali persidengti
	
	for (int i = 0; i < trynimui.size(); i++) {		//ca visas sarasas kuriuos reikia istrinti is poolo

		for (int j = 0; j < TransPool.size(); j++) {	//cia iesko kuri reikia istrinti per visa poola

			if (trynimui.at(i).ID == TransPool.at(j).ID) {

				TransPool.erase(TransPool.begin() + j);
			}
		}
	}


}

void BlockGenerator(int x) {

	Block Candidates[5];  //penki kandidatai

	int panaudotiblokai[5];

	int  tikrinti, ran, tik = 0;
	bool paleidimas = true;


	for (int i = 0; i < x; i++) {		//vienas sitas prasukimas prideda viena bloka i lista

		for (int i = 0; i < 5; i++) {
			panaudotiblokai[i] = 0;		//pradzioje panaudotu bloku nera
		}

		for (int j = 0; j < 5; j++) {

			Candidates[j].AtrinktosT = ChoosingTransactions();
			Candidates[j].Merkel = MerkelRoot(Candidates[j].AtrinktosT);	//sukuriami kandidatai
			Candidates[j].TimeStamp = std::time(0);
			Candidates[j].DifficultyTarget = 5;
			Candidates[j].Nonce = 1;
			Candidates[j].Hash = "";

		}

		tikrinti = 100000;	 //kiek max tikrinimu

		paleidimas = true;

		while (paleidimas) {

			while (true) {
				ran = std::round(0 + (double)rand() / RAND_MAX * 4);	//random blokas
				if (panaudotiblokai[ran] == 0) {	//jei blokas dar netikrintas, breakina
					break;
				}
			}

			//blokas atrinktas, dabar 
			if (h == NULL) Candidates[ran].PreviousHash = "0"; //jei pirmas blokas, jo previous hashas yra 0
			else Candidates[ran].PreviousHash = t->Hash;	//jei ne pirmas, t ziures i praeita hasha


			std::string Bhash, a1, a2; //bhash - galutinis bloko hashas

			for (int i = 0; i < tikrinti; i++) {
				a1 = sha256(Candidates[ran].Merkel + std::to_string(Candidates[ran].TimeStamp)); //a1 ir a2 yra tarpiniai
				a2 = sha256(Candidates[ran].PreviousHash + std::to_string(Candidates[ran].Nonce));
				Bhash = sha256(a1 + a2);

				tik = 0;

				for (int j = 0; j < Candidates[ran].DifficultyTarget; j++) { // tikrina, ar sukurtas hashas atitinka nustatyta difficulty

					if (Bhash.at(j) == '0') tik++; //suskaiciuoja, kiek atitinka
				}
				if (tik != Candidates[ran].DifficultyTarget) {	//cia patikrina, ar visi kiek reikia atitinka

					Candidates[ran].Nonce = Candidates[ran].Nonce + 1; //jei neatitinka visi, keiciamas nonce, del jo keisis bendras hashas
				}
				else {
					Candidates[ran].Hash = Bhash;	//jei viskas tinka, tas blokas gauna hasha
					i = tikrinti;	//cia breakinamas 100k ciklas
				}

			}

			if (Candidates[ran].Hash == "") {	//jei patikrino 100k kartu ir hasho vis tiek nerado
				panaudotiblokai[ran] = 1;
			}
			else {
				n = new Block;			//tinkamas blokas perkuopijuojamas i lista
				n->AtrinktosT = Candidates[ran].AtrinktosT;
				n->Merkel = Candidates[ran].Merkel;
				n->TimeStamp = Candidates[ran].TimeStamp;
				n->DifficultyTarget = Candidates[ran].DifficultyTarget; //kiek nuliu pradzioje
				n->Nonce = Candidates[ran].Nonce;
				n->PreviousHash = Candidates[ran].PreviousHash;
				n->Hash = Candidates[ran].Hash;
				if (i == 0) h = n;
				t = n;		//t ziuri i katik sukurta bloka, kita pasukima jis ziures i praeita bloka
				//n->Hash = BlockHash();
				std::cout << i + 1 << " blokas" << std::endl << "Hashas: " << n->Hash << std::endl << "Nonce: " << n->Nonce << std::endl << "PrevH: " << n->PreviousHash << std::endl << "AtrinktosT: " << n->AtrinktosT.size() << std::endl;
				paleidimas = false;
				TransactionDelete(n->AtrinktosT);	//istrina panaudotas transakcijas, kad nesidubliuotu per blokus NE kandidatus

				std::cout << "Transaction likutis: " << TransPool.size() << std::endl << std::endl;
			}

			if (panaudotiblokai[0] == 1 && panaudotiblokai[1] == 1 && panaudotiblokai[2] == 1	//jei ne vieno bloko nesurado
				&& panaudotiblokai[3] == 1 && panaudotiblokai[4] == 1) {

				panaudotiblokai[0] = 0;
				panaudotiblokai[1] = 0;
				panaudotiblokai[2] = 0;
				panaudotiblokai[3] = 0;
				panaudotiblokai[4] = 0;

				tikrinti = tikrinti*2;	//padvigubina sukimu skaiciu
			}

			
		}
	}

	

	/*for (int i = 0; i < x; i++){
			n = new Block;
			n->AtrinktosT = ChoosingTransactions(); //cia pasirinks 100 atsitiktiniu transakciju vienam blokui
			n->Merkel = MerkelRoot(n->AtrinktosT);
			n->TimeStamp = std::time(0);
			n->DifficultyTarget = 3; //kiek nuliu pradzioje
			n->Nonce = 1;
			if (i == 0) n->PreviousHash = "0"; //jei pirmas blokas, jo previous hashas yra 0
			else n->PreviousHash = t->Hash;		//jei ne pirmas, jis paima praeito bloko hasha, t ziuri i sena bloka, n ziuri i nauja
			if (i == 0) h = n;
			else t->next = n;
			t = n;
			//n->Hash = BlockHash();
			std::cout << i << " blokas" << std::endl;
	} */
	
}




int main() {
		
		UserGenerator();
		TransactionGenerator();
		BlockGenerator(50); 
		std::cout << "Sugeneruoti visi blokai " << std::endl;

	return 0;
}
