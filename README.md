# Block_2 
### [v0.1]() 2020-11-13
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
