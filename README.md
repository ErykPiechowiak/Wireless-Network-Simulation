# Wireless-Network-Simulation
Symulacja sieci bezprzewodowej opartej na protokole CSMA/CA

Projekt powstał w czerwcu 2020 roku w ramach realizacji zajęć z przedmiotu dotyczącego symulacji cyfrowych. 

Treść rozwiązywanego problemu:

W sieci bezprzewodowej stacje nadawcze konkurują o dostęp do łącza.
W losowych odstępach czasu CGPk k-ta stacja nadawcza generuje pakiety gotowe do wysłania.
Po uzyskaniu dostępu do łącza zgodnie z algorytmem CSMA/CA, k-ty terminal podejmuje próbę transmisji najstarszego pakietu ze swojego bufora. Czas transmisji wiadomości z k-tej stacji nadawczej do k-tej stacji odbiorczej wynosi CTPk. 
Jeśli transmisja pakietu zakończyła się sukcesem, stacja odbiorcza przesyła potwierdzenie ACK (ang. Acknowledgment) poprawnego odebrania wiadomości.
Czas transmisji ACK wynosi CTIZ. Jeśli transmisja pakietu nie powiodła się, stacja odbiorcza nie przesyła ACK. Odbiór pakietu uznajemy za niepoprawny, jeśli w kanale transmisyjnym wystąpiła kolizja lub błąd.
Przez kolizję rozumiemy nałożenie się jakiejkolwiek części jednego pakietu na inny pakiet (pochodzący z innego nadajnika). Dodatkowo każda transmisja pakietu może zakończyć się błędem TER.
Brak wiadomości ACK po czasie (CTPk+CTIZ) od wysłania pakietu jest dla stacji nadawczej sygnałem o konieczności retransmisji pakietu. Każdy pakiet może być retransmitowany maksymalnie LR razy.
Dostęp do łącza w przypadku retransmisji opiera się na tych samych zasadach co transmisja pierwotna.
Jeśli mimo LR-krotnej próby retransmisji pakietu nie udało się poprawnie odebrać, wówczas stacja nadawcza odrzuca pakiet i –jeśli jej bufor nie jest pusty –przystępuje do próby transmisji kolejnego pakietu.

Opracuj symulator sieci bezprzewodowej zgodnie z metodą ABC.

Za pomocą symulacji wyznacz:

● Wartość parametru L, która zapewni średnią pakietową stopę błędów (uśrednioną po K   odbiornikach) nie większą niż 0.1, a następnie:

o pakietową stopę błędów w każdym z odbiorników mierzoną jako iloraz liczby pakietów straconych do liczby przesłanych pakietów,

o średnią liczbę retransmisji pakietów,

o przepływność systemu mierzoną liczbą poprawnie odebranych pakietów w jednostce czasu,

o średnie opóźnienie pakietu, tzn. czas jaki upływa między pojawieniem się pakietu w buforze, a jego poprawnym odebraniem,

o średni czas oczekiwania, tzn. czas między pojawieniem się pakietu w buforze, a jego opuszczeniem

o Sporządź wykres zależności średniej liczby retransmisji pakietów od parametru P Sporządź wykres zależności przepływności systemu oraz średniej i maksymalnej pakietowej stopy błędów w zależności od wartości L.
	


Parametry

CGPk - zmienna losowa o rozkładzie wykładniczym o intensywności 
LCTPk–zmienna losowa o rozkładzie jednostajnym w przedziale {1, 2, ..., 10}ms
R –zmienna losowa o rozkładzie jednostajnym w przedziale <0, (2r-1)>
T –zmienna losowa o rozkładzie jednostajnym w przedziale <0, (2t-1)>
TER -zmienna losowa o rozkładzie zero-jedynkowym z prawdopodobieństwem sukcesu P
CTIZ= 1 ms
CSC= 1 ms
P = 0.8

Protokół dostępu do łącza:

Protokół CSMA/CA (ang. Carrier Sense Multiple Access/Collision Avoidance–po wygenerowaniu nowego pakietu, stacja nadawcza sprawdza zajętość kanału transmisyjnego (nasłuchiwanie kanału odbywa się co 0.5 ms). Jeśli kanał jest wolny przez okres dłuższy niż czas DIFS, to podejmuje próbę przesłania swojego pakietu. Jeśli natomiast medium jest zajęte, nadawanie jest wstrzymane do czasu wykrycia, że kanał jest wolny przez czas DIFS, po czym rozpoczyna się proces losowego opóźnienia transmisji. Terminal losuje wartość licznika BT (ang. Backoff Timer), będącego liczbą z przedziału <0,CW>, gdzie CW (ang. Contention Window) jest szerokością okna rywalizacji i mieści się w przedziale <CWmin, CWmax> (początkowo CW = CWmin). Następnie w odstępach czasu co 0.5 ms, terminal przepytuje kanał. Jeśli kanał jest wolny, to licznik BT zmniejszany jest o 1. Jeśli kanał jest zajęty, to procedura jest zawieszona i wznowiona po wykryciu, że kanał jest wolny. Stacja nadaje pakiet w momencie, w którym licznik BT osiągnie wartość 0.
W przypadku każdej retransmisji jest uruchamiany proces losowego opóźnienia transmisji. Wartość licznika BT jest losowana z przedziału <0, CW>, przy czym CW= min(2(CW+1)-1, CWmax) (CW podwaja się po każdej nieudanej transmisji). Jeśli w poprzedniej retransmisji CW osiągnęło wartość CWmax, transmitowany pakiet uznajemy za stracony i nie ponawiamy próby jego retransmisji.

Parametry:
DIFS = 3 ms
CWmin = 15
CWmax = 255
K = 10
