# Block_2 
### [v0.1](https://github.com/GudUgne/Block_2/releases/tag/v0.1) 2020-11-13
 :money_with_wings: Second task of the Blockchain course :money_with_wings:

- v0.1 versijoje buvo reikalaujama, jog naudotumėme savo pirmos užduoties metu sukurtą hash'ą, tačiau manasis nebuvo tinkamas. 
Antrajai užduočiai tūrėtų būti nustatomas difficulty target - hash'ų tam tikras priekyje esančių simbolių kiekis turi sutapti, pvz. visi prasideda 0000..ir t.t.
Mano hash'as to padaryti negali, jis negeneruoja hashų su vienodais simboliais;

Žemiau yra patikrinimas, kai lyginau ar hash'ų pirmi, antri, n-ti simboliai sutampa su kitų hash'ų 1, 2, n-tais simboliais:
![Screenshot](https://github.com/GudUgne/Block_2/blob/v0.1/lyginimas%20hashu.png)

- Dėl šios priežasties antrajai semestro užduočiai naudosiu SHA256 hash'ą, rastą internete;

- Yra atskiros User, Transaction ir Block Generator funkcijos, Useriams generuoti sukuriamas globalus struktūrų masyvas, o Blokui naudojamas linked list'as. Transakcijoms naudojamas struktūrų vektorius;
 
- Yra atskira transakcijų (random) parinkimo funkcija, kurioje yra laikinas transakcijų vektorius, į jį dedamos atsitiktinės transakcijos iš poolo, tada tas laikinas vektorius pereina jau i bloko struktūroje paskirtą vektorių;

- Pirmoje kodo versijoje jau yra implementuotas Merkle medis;

## :money_with_wings: Pseudokodas: :money_with_wings:

## Sugeneruojami useriai naudojant globalų struktūrų masyvą:
- User numeris (name) generuojamas nuo 1 iki 1000;
- Tada generuojamas 32 simbolių (nuo 0-f) public key;
- Generuojama random suma, kurią turi useris;
  
## Sugeneruojamos 10000 transakcijų naudojant globalų struktūrų vektorių:
- Paimamas vienas atsitiktinis useris kaip siuntėjas;
- Sugeneruojama, kiek jis pinigų siunčia, nuo 1 iki max, kiek turi;
- Atimami pinigai iš jo sąskaitos;
- Generuojamas atsitiktinis gavėjas - jei gavėjas ir siuntėjas yra tas pats asmuo, gavėjas generuojamas vėl;
- Į transakcijos ID "suklijuojami" ir vėliau ušhashuojami sumos, siuntėjo ir gavėjo string'ai;

## Bloko generavimas (iš jo išplaukia daug funkcijų):
- Blokui kurti naudojamas linked listas;
- Pirma pasirenkamos 100 atsitiktinių transakcijų:
  -    Paimama atsitikinė transakcija iš poolo;
  -   Perkeliama į laikiną vektorių;
  -   Kai surenkama 100 transakcijų, jos keliauja į bloke paskirtą transakcijų vektorių;
- Po šio žingsnio eina Merkel medis:
  -    Į funkciją paduodamos atrinkos transakcijos;
  -   Sukuriami du laikini vektoriai - pirmas dar nesutvarkytom transakcijom (100), o antras, jau poromis suhashintoms transakcijoms (jau lieka 50)
  -   Kai antrame vektoriuje suhashinamos transakcijos, jos keliauja į pirmą, tada procesa vėl kartojamas iš naujo;
  -   Hashuojamų transakcijų skaičius gal būti nelyginis, tuo atveju paskutinė transakcija ušhashuojama pati su savimi;
  -   Funkcijos pabaigoje lieka vienas hashas, jis grąžinamas į bloko generavimo funkciją;
- Po merkel eina timestamp, kuris paima dabartinį laiką; 
- Nustatomas sugalvotas difficulty target;
- Nonce nustatomas kaip 1 (padidinamas vėliau);
- Pažiūrima, ar tai pirmas generuojamas blokas, jei taip, previous hashas yra 0 (nes previous tiesiog nėra);
- Jei tai ne pirmas blokas, tai to bloko previous hashas yra lygus ankstesnio bloko hashui;
- Pabaigoje kviečiama BlockHash funkcija:
  - Ušhashuojamas timestampas, vėliau nonce;
  - Ušhashavus jie sudedami į galutinį hasha, kurį jau grąžins funkcija;
  - Tikrinama, ar sukurtas hasha atitinka difficulty target;
  - Jei neatitinka, keičiamas nonce, funkcija vėl pradedama iš naujo;
- Jau gavus tinkamą bloko hashą, išspausdinama, jog bloko kūrimas yra baigtas;

## Trukmė
- Blokas sugeneruojamas maždaug per sekundę;
