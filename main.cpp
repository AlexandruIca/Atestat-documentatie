///
/// @file
///
/// @mainpage Omida
///
/// Omida este un joc care seamana foarte tare cu Snake. Nu l-am putut numi snake
/// pentru ca numele era deja luat :(.
///
/// - @subpage cum_se_joaca
///
/// - @subpage cum_functioneaza
///
/// - @subpage la_ce_ajuta
///
/// - @subpage extra
///

///
/// @page cum_se_joaca Cum se joaca?
///
/// - @ref group_meniu_start
/// - @ref group_jocul_efectiv
/// - @ref group_pauza
/// - @ref group_final
///

/// 
/// @defgroup group_meniu_start Meniu start
///
/// La inceput jocul afiseaza un meniu cu 2 optiuni: joaca si iesire.
///
/// Meniul selectat are un fundal mov inchis iar pentru a intra in meniul
/// respectiv se apasa tasta ENTER. Pentru a selecta alt meniu fie se apasa
/// click pe meniul pe care il vreti, fie folositi sagetile, fie tastele
/// j si h(j - jos, k - sus).
///
/// Pentru a iesi din joc(in meniul de start) in afara de a apasa "iesire"
/// se poate apasa tasta 'x' sau ESCAPE.
///
/// Odata ce ati selectat "joaca" va incepe jocul efectiv.
///

///
/// @defgroup group_jocul_efectiv Jocul efectiv
///
/// Jocul va incepe cu o omida mica asezata aproximativ in centrul ferestrei.
/// Pe capul omizii este afisat scorul curent(care incepe cu 0). Omida se
/// misca automat in directia setata de jucator la un interval de 150 de
/// milisecunde, reprezentat de variabila @ref StadiulJocului::timp_asteptare_maxim.
///
/// Initial directia in care se va deplasa omida este @ref TipDirectie::SUS. Toate
/// directiile sunt in enumeratia @ref TipDirectie. Pentru a schimba directia
/// omizii se folosesc fie sagetile, fie tastele h, j, k, l(inspirate din vim,
/// h - @ref TipDirectie::STANGA, j - @ref TipDirectie::JOS, k - @ref
/// TipDirectie::SUS, l - @ref TipDirectie::DREAPTA.
///
/// De asemenea pe undeva in teren este afisata si o frunza care va incrementa
/// scorul cu 1 daca veti ajunge la ea. Cand "mancati" o frunza veti auzi si 
/// sunetul reprezentat de variabile @ref Joc::m_sunet_frunza. Exista si un
/// powerup in joc(adica un mar) care va incrementa scorul cu 10. Marul este
/// generat odata la 20 de secunde(evident, daca nu este deja in teren).
/// Sunetul redat la "mancarea" unui mar este reprezentat de variabila
/// @ref Joc::m_sunet_powerup.
///
/// Terenul are dimensiunile 20x20 ceea ce inseamna ca intervalul de 150ms
/// poate fi un pic cam mare. Pentru a misca omida mai repede tineti apasat
/// tastele cu care schimbari directia. Cat tineti apasat tastele respective
/// acele 150ms vor deveni 50ms.
///
/// Pentru a face jocul un pic mai complicat daca ajugeti la marginile terenului
/// omida va muri. In multe variante de snake sarpele nu moare daca atinge
/// peretele ci va continua pe partea opusa. Omida noastra poate muri si
/// daca a ajuns la margine si daca s-a lovit de propiul ei corp.
///

///
/// @defgroup group_pauza Pauza
///
/// Jocul poate fi oprit temporar, daca apasati tasta ESCAPE. Ca sa reluati
/// jocul apasati din nou tasta ESCAPE. Daca vreti sa iesiti din joc in timp
/// ce sunteti in pauza apasati tasta 'x'.
///

///
/// @defgroup group_final Final
///
/// Odata ce omida a murit sau ati iesit din joc in timpul unei pauze veti fi 
/// intampinat de un nou meniu care va afisa scorul obtinut si scorul maxim de
/// pana acum si cine il detine. Daca ati depasit scorul maxim vi se va cere
/// numele pentru a retine noul maxim. Pentru a reveni la meniul principal
/// apasati enter.
///

///
/// @page cum_functioneaza Cum functioneaza?
///
/// - @ref group_deseneaza_pe_ecran
/// - @ref group_cum_reprezint_obiectele
/// - @ref group_meniuri
/// - @ref group_main_loop
///

///
/// @defgroup group_deseneaza_pe_ecran Desenand pe ecran
///
/// In primul rand pentru a putea desena orice pe ecran in c++(si nu numai)
/// trebuie sa creem o fereastra. Procesul de a crea o fereastra este specific
/// fiecraui sistem de operare. In windows avem la dispozitie functia
/// CreateWindowEx, in lumea Linux poate fi creata cu ajutorul Xlib/XCB sau
/// Wayland, iar pe Mac OS o varianta ar fi Cocoa(desi nu este nativ c++
/// poate fi creat un wrapper cu ajutorul llvm-clang). Jocul meu suporta toate
/// aceste platforme(poate chiar si android si ios) si totusi nu trebuie
/// sa contina cod pentru fiecare platforma in parte. Asta datorita faptului
/// ca foloseste o librarie numita SDL care a facut deja asa ceva. Libraria
/// este inclusa in proiect. In plus imi ofera si functii pentru a desena
/// imagini si figuri geometrice simple. Daca nu m-as fi folosit de o astfel
/// de librarie ar fi trebuit sa folosesc opengl care este suportat pe
/// multe platforme(dar de ceva timp nu mai este suportat pe ios is Mac OS) sau sa
/// recurg la mai mult cod care depinde de platforme(direct3d - windows, 
/// Metal/Quartz - MacOs/ios etc.).
///
/// Codul care deschide o fereastra se gaseste in functia @ref initializeaza.
/// Prima data chem functia SDL_Init care va returna o valoarea negativa daca
/// va esua, apoi creez efectiv fereastra cu ajutorul SDL_CreateWindow. Primul
/// parametru este titlul ferestrei, al doilea si al treilea reprezinta
/// coordonatele unde va fi afisata fereastra, al patrulea si al cincilea
/// reprezinta lungime si inaltimea ferestrei, iar ultimul nu este atat de
/// important.
///
/// Odata ce am deschis fereastra trebuie sa "activam" posibilitatea de a
/// desena pe ea. Variabila @ref global::desenator este un pointer catre
/// un obiect de tip SDL_Renderer, iar pentru a crea efectiv desenatorul
/// chem functia SDL_CreateRenderer care ia ca parametrii fereastra pe care
/// vreau sa desenez si anumite "steaguri" pe care vreau sa le setez. In cazul
/// meu setez SDL_RENDERER_ACCELERATED care va folosi accelerarea hardware daca
/// va putea, ceea ce va imbunatati performanta considerabil.
///
/// Mai jos chem TTF_Init si Mix_OpenAudio care imi ofera posibilitatea de
/// a desena text pe ecran si de e reda sunet.
///

///
/// @defgroup group_cum_reprezint_obiectele Cum reprezint obiectele jocului?
///
/// Prima data vom avea nevoie de un mod de a reprezenta omida. Cel mai usor ar
/// fi sa folosim un vector care tine pozitii ale omizii, primul/ultimul element
/// fiind capul omizii. Eu folosesc ceva similar dar care imi ofera mult mai
/// multa flexibilitate si care este si mai rapid si consuma mai putina memorie,
/// si anume std::deque. Deque inseamna "double ended queue" in engleza si
/// care este un fel de coada dar care este foarte buna la insertia la inceput
/// si la sfarsit - exact ce imi trebuie. In spate deque este o lista de vectori
/// cu o marime fixa(de exemplu 5) care vrea sa aiba avantajele unui vector de
/// a avea memorie continua si foarte rapid de accesat dar si avantajele unei
/// liste de a fi foarte eficienta la insertia in mijlocul unui sir.
///
/// Omida este stocata in variabila @ref StadiulJocului::pozitii_omida. Tipul
/// de date folosit pentru a stoca efectiv pozitiil este @ref Punct. De remarcat
/// este faptul ca retin pozitiile unei matrici(care reprezinta terenul) @ref
/// StadiulJocului::teren, nu valorile absolute in fereastra. Acest lucru face
/// mult mai usoara logica jocului.
///
/// Cand jocul incepe sunt inserate 3 pozitii in omida. Cand este timpul ca
/// pozitia omizii sa fie actualizata chem @ref Joc::muta_omida. La fiecare pas
/// chem @ref deseneaza_omida, care parcurge @ref StadiulJocului::pozitii_omida
/// si deseneaza fiecare parte a omizii pe ecran.
///
/// Pentru a usura munca de a schimba directia omizii folosescu o enumeratie
/// @ref TipDirectie. Apoi am un vector @ref directie care va tine valorile
/// cu care trebuie sa incrementez i-ul si j-ul omizii pentru a se deplasa.
/// Cand omida se deplaseaza inserez la inceputul cozii noua pozitie a capului
/// in functie de directie. Daca nu a mancat nimic cat s-a deplasat atunci
/// sterg ultima pozitie din coada.
///
/// Pozitia frunzei este retinuta in variabila @ref
/// StadiulJocului::pozitie_frunza. Cand omida mananca o frunza trebuie
/// generata o noua pozitie, astfel chem functia @ref genereaza_pozitie_noua.
/// Aceeasi functie este chemata si atunci cand omida mananca un mar.
///
/// Daca jucatorul apasa tasta escape atunci chem functia @ref Joc::pauza.
///

///
/// @defgroup group_meniuri Meniuri
///
/// Jocul in sine nu este prea complicat. Snake este un joc arhipopular care
/// are multe implementari. Totusi, problema intervine cand vrem sa avem si
/// alte "auxiliare" pentru joc, cum ar fi un meniu de start si final. Cum
/// putem tine separat codul pentru a desena meniuri si cel care tine de
/// jocul efectiv?
///
/// O metoda naiva ar fi sa verificam pur si simplu prin niste if-uri unde ne
/// aflam. Ceva de genul:
/// @code{.cpp}
///  if(este_in_meniu_start) { /* ... */ }
///  else if(este_in_joc) { /* .. */ }
///  /* etc */
/// @endcode
///
/// Metoda naiva functioneaza dar este foarte limitata(si urata). Jocurile mult
/// mai complexe care trebuie sa deseneze un meniu separat numai pentru optiunile
/// jocului in timp ce are alte submeniuri nu poate functiona asa. Atunci am
/// implementat conceptul de scene.
///
/// O scena este practic un joc intreg. Meniul de start are propria scena,
/// jocul in sine are propria scena, meniul de final alta scena, etc.
/// Mai exact, fiecare scena este o clasa care trebuie sa mosteneasca @ref
/// Scena. O scena trebuie sa aibe 3 functii implementate:
/// - incarca
/// - incarcat_deja
/// - itereaza
///
/// @ref Scena::incarca face o initializare a scenei, de exemplu in scena @ref
/// Joc functia @ref Joc::incarca incarca toate imaginile pentru omida, frunza,
/// mar, initializeaza matricea jocului etc.
///
/// @ref Scena::incarcat_deja este evidenta.
///
/// @ref Scena::itereaza este apelata la fiecare pas in @ref group_main_loop.
///
/// Scenele sunt stocate in @ref global::scene intr-o coada, la fel ca @ref
/// StadiulJocului::pozitii_omida sub forma unor pointeri. @ref Scena
/// este o clasa abstracta deci nu poate fi create direct, decat prin
/// mostenitori, deci trebuie folositi pointeri. Astfel avem un sistem flexibil
/// prin care putem reprezenta scene. Vedeti cum sunt folosite exact in @ref
/// group_main_loop.
///

///
/// @defgroup group_main_loop Principala repetitie
///
/// In while-ul din functia @ref main se intampla practic tot. Inainte de
/// while sunt inserate doua obiecte de tip @ref Joc si @ref MeniuStart.
/// Scenele sunt folosite ca un fel de stiva, ultima scena fiind cea care
/// trebuie redata. De aceea este inserata prima data scena jocului. Cand vom
/// iesi din meniul de start va fi sters si va ramane scena jocului. Orice
/// scena poate iesi complet din aplicatie daca seteaza @ref
/// global::fereastra_inchisa la fals. O scena va fi stearsa numai daca a fost
/// initializata, si va fi initializata numai daca nu s-a intamplat deja.
///

///
/// @page la_ce_ajuta La ce ajuta?
///
/// Un joc in sine nu are ata de multe aplicatii in lumea reala in afara de
/// entertainment. Totusi consider ca acest proiect reprezinta un foarte bun
/// inceput pentru oricine vrea sa inceapa programarea grafica. Aceasta are mult
/// extrem de multe aplicatii precum vizualizarea organelor corpuli uman,
/// simulari, design etc. "Omida" nu este ceva complex dar este o fundatie
/// pentru a dezvolta subiecte mai avansate. Chiar si conceptul scenelor intr-un
/// joc este foarte important deoarece cand vrem sa facem un joc nu ne gandim
/// prima data la meniu ci la joc in sine. Dupa ce am terminat jocul intervine
/// problema de reprezentare a meniurilor, conceptul de scena ajutand enorm.
/// Scenele pot fi extinse si reprezentate ca niste arbori pentru flexibilitate
/// maxima(de exemplu submeniuri). De asemenea poate fi extins si modul in
/// care functioneaza @ref main, in loc sa chemam fiecare @ref Scena::itereaza
/// cu @ref StadiulJocului::timp_trecut variabil, putem stabili un timp maxim
/// (de exemplu 60fps) cu care sa actualizam scena, folositor in cazul unei
/// simulari in care legile fizicii depind foarte mult de timpul trecut.
///

///
/// @page extra Extra
///
/// Codul sursa este complet liber, poate fi gasit la 
/// https://gitlab.com/librehead/atestat si se afla sub licenta Apache 2.0.
/// Libraria SDL este si ea open source sub licenta zlib. Documentatia am
/// generat-o cu ajutorul doxygen, de asemenea proiect open source, sub
/// licenta GNU General Public License.
///

#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

///
/// O structura ajutatoare pentru a reprezenta o pozitie in matricea jocului.
///
struct Punct
{
    /// 
    /// @brief Linia in matrice.
    /// 
    int i{ 0 };
    ///
    /// @brief Coloana in matrice.
    ///
    int j{ 0 };

    ///
    /// Vrem ca o variabila de tipul @ref Punct sa aiba valorile initiale 
    /// i = 0 si j = 0 asa ca nu ar trebui modificat nimic in constructor.
    ///
    /// Exemplu:
    /// @code{.cpp}
    ///     Punct a;
    ///     a.i == 0; // adevarat
    ///     a.j == 0; // adevarat
    /// @endcode
    ///
    Punct() = default;
    ///
    /// Putem declara un punct si cu valori customizate pe loc, exemplu:
    /// @code{.cpp}
    ///     Punct a{ 2, 3 }; // a.i va fi 2 si a.j va fi 3
    ///     Punct b(2, 3); // la fel ca a
    /// @endcode
    ///
    Punct(int t_i, int t_j)
        : i{ t_i }
        , j{ t_j }
    {}
    ///
    /// Declarat pentru a putea copia valorile unui punct in alta variabila, exemplu:
    /// @code{.cpp}
    ///     Punct a{ 2, 3 };
    ///     Punct b{ 5, 6 };
    ///     b = a; // acum b.i = 2 si b.j = 3
    /// @endcode
    ///
    Punct(const Punct&) = default;
};

///
/// @brief Unde se poate deplasa omida.
///
enum TipDirectie
{
    SUS = 0, 
    JOS = 1, 
    STANGA = 2, 
    DREAPTA = 3
};

///
/// Pentru a face mutarea omizii mai usoara pozitiile capului si cozii vor 
/// fi actualizate cu pozitiile lor curente + directie[STANGA] sau DREAPTA etc.
/// @code{.cpp}
///     Punct cap{ 2, 3 };
///     // Pentru a muta la stanga:
///     cap.i += directie[STANGA].i;
///     cap.j += directie[STANGA].j;
/// @endcode
///
Punct directie[] = {
    { -1,  0 }, // SUS
    {  1,  0 }, // JOS
    {  0, -1 }, // STANGA
    {  0,  1 }  // DREAPTA
};

///
/// Terenul este o matrice care contine una din urmatoarele posibilitati de mai jos.
///
enum PosibilitateTeren
{
    GOL = 0, 
    OMIDA, 
    FRUNZA,
    POWERUP
};

class Scena;

///
/// Contine variabile de care putem avea nevoie oricand, indiferent de stadiul jocului.
///
namespace global {
    ///
    /// @brief Fereastra in care vom desena tot.
    ///
    SDL_Window* fereastra{ nullptr }; 
    ///
    /// Nu putem desena direct in fereastra dar SDL ne ofera posibilitatea sa creem 
    /// un 'desenator' care va desena efectiv figuri geometrice/texturi.
    ///
    SDL_Renderer* desenator{ nullptr };

    ///
    /// Devine adevarat cand jucatorul inchide fereastra de tot.
    ///
    bool fereastra_inchisa = false; 

    ///
    /// @brief Lungimea ferestrei.
    ///
    constexpr int lungime = 600;
    ///
    /// @brief Inaltimea ferestrei.
    ///
    constexpr int inaltime = 600;

    ///
    /// Toate scenele sunt pastrate in acest vector.
    /// Clasa @ref Scena ofera o interfata comuna pe care orice scena(care e 
    /// de fapt o alta aplicatie, de exemplu meniul de inceput/pauza) trebuie sa o 
    /// urmeze. Utilizarea arata cam asa:
    /// @code{.cpp}
    ///     while(!global::fereastra_inchisa) {
    ///         global::scene.back()->incarca();
    ///         
    ///         while(global::scene.back()->itereaza()) {}
    ///         
    ///         global::scene.back()->iesire();
    ///     }
    /// @endcode
    ///
    std::deque<std::unique_ptr<Scena>> scene;
    ///
    /// De fiecare data cand jucatorul apasa o tasta aceasta e retinuta in aceasta mapa.
    /// Mapa este un fel de vector dar care poate avea altfel de indexare.
    /// In cazul acesta ma pot referi la tasta escape asa:
    /// @code{.cpp}
    ///     if(global::taste_apasate[SDL_SCANCODE_ESCAPE]) {
    ///         // a apasat escape
    ///     }
    /// @endcode
    ///
    std::map<SDL_Scancode, bool> taste_apasate;
}

///
/// @brief Contine toata informatia de care avem nevoie in timpul jocului.
///
class StadiulJocului
{
public:
    /// 
    /// @brief Lungimea matricii jocului.
    ///
    static constexpr int lungime_teren = 20;
    /// 
    /// @brief Inaltimea matricii jocului.
    ///
    static constexpr int inaltime_teren = 20;

    ///
    /// @brief Lungimea unei texturi.
    /// 
    /// In functie de dimensiunea matricii jocului lungimea si inaltimea texturilor se
    /// schimba. In cazul nostru o textura nu e altceva decat o imagine incarcata si desenata
    /// in fereastra. 
    ///
    static constexpr int lungime_textura = global::lungime / lungime_teren;
    ///
    /// @brief Inaltimea unei texturi.
    ///
    static constexpr int inaltime_textura = global::inaltime / inaltime_teren;

    ///
    /// @brief Matricea jocului.
    ///
    /// Matricea reprezinta terenul pe care se deplaseaza omida. Valorile din matrice pot fi 
    /// una dinre @ref PosibilitateTeren. Omida este reprezentata ca o coada care contine
    /// @ref Punct, o coada in care se poate insera si la inceput si la final: pozitii_omida.
    ///
    PosibilitateTeren teren[inaltime_teren][lungime_teren];
    ///
    /// @brief Coada care contine pozitiile omizii.
    ///
    /// Este o coada in care se poate insera si la inceput si la final. Prin pozitiile omizii
    /// se intelege unde se afla cercurile cu care desenez omida. Omida va fi reprezentata
    /// cu cercuri, capul este un cerc rosu, restul cercuri albastre. Astfel parcurg coada pentru
    /// a determina unde trebuie sa desenez cercurile.
    /// pozitii_omida.front() va fi capul omizii.
    ///
    std::deque<Punct> pozitii_omida;
    ///
    /// @brief Unde se afla frunza.
    ///
    Punct pozitie_frunza;
    ///
    /// @brief Unde se afla powerup-ul.
    ///
    /// Un Powerup este un mar, mancare mult mai consistenta pentru omida.
    ///
    Punct pozitie_powerup;
    ///
    /// @brief Adavarat daca am generat deja un powerup.
    ///
    bool exista_powerup = false;
    ///
    /// @brief Scorul jocului.
    ///
    /// Pentru fiecare frunza mancate scorul creste cu 1.
    /// 
    int scor = 0;

    ///
    /// @brief Timpul care a trecut de la ultima 'desenare'.
    ///
    long long timp_trecut = 0;
   
    ///
    /// @brief Timpul maxim pana cand omida se va misca.
    ///
    static long long timp_asteptare_maxim;

    ///
    /// @brief In ce directie se deplaseaza omida.
    ///
    /// Initial se deplaseaza in sus dar se va schimba in functie de ce taste apasa
    /// jucatorul.
    ///
    TipDirectie directia_omizii = TipDirectie::SUS;
   
    SDL_Texture* textura_capului{ nullptr };
    SDL_Texture* textura_corpului{ nullptr };
    SDL_Texture* textura_frunzei{ nullptr };
    SDL_Texture* textura_powerup{ nullptr };

    SDL_Color culoare_fundal = { 15, 15, 15, 255 };
};

long long StadiulJocului::timp_asteptare_maxim = 150LL;

///
/// @brief Interfata pentru toate scenele.
///
class Scena
{
public:
    ///
    /// Initializeza scena.
    ///
    virtual void incarca(StadiulJocului&) = 0;
    ///
    /// Verifica daca scena curenta a fost incarcata deja.
    ///
    virtual bool incarcat_deja() = 0;
    ///
    /// @brief Paraseste scena.
    ///
    virtual void iesire(StadiulJocului&) = 0;
    ///
    /// @brief Executa functia la fiecare 'desenare'.
    ///
    /// @return Adevarat daca iterarea va continua, fals altfel.
    ///
    virtual bool itereaza(StadiulJocului&) = 0;

    virtual ~Scena() = default;
};

///
/// @brief Scena jocului in sine.
///
class Joc : public Scena
{
    ///
    /// Cand depaseste @ref StadiulJocului::timp_asteptare_maxim omida trebuie mutata.
    ///
    long long m_timp_asteptat = 0LL;
    ///
    /// Pana cand omida trebuie mutata actualizeaza directia. De exemplu, jucatorul apasa
    /// tasta UP, dar dupa apasa DOWN fara ca @ref Joc::m_timp_asteptat sa fi depasit
    /// @ref StadiulJocului::timp_asteptare_maxim.
    /// Cand omida trebuie mutata actualizeaza @ref StadiulJocului::directia_omizii cu 
    /// directia noua.
    ///
    TipDirectie noua_directie_a_omizii = TipDirectie::SUS;
    
    bool m_incarcat_deja = false;

    ///
    /// @brief Fontul folosit pentru a desena text pe ecran.
    ///
    TTF_Font* m_font = nullptr;

    ///
    /// @brief Sunetul redat de fiecare data cand omida mananca o frunza.
    ///
    Mix_Chunk* m_sunet_frunza = nullptr;
    /// 
    /// @brief Sunetul redat de fiecare data cand omida mananca un powerup.
    ///
    Mix_Chunk* m_sunet_powerup = nullptr;

    ///
    /// Muta omida in directia setata de jucator.
    ///
    /// @return Adevarat daca omida nu a murit, fals altfel.
    ///
    bool muta_omida(StadiulJocului&);
    ///
    /// Ruleaza cat timp jocul e in pauza.
    ///
    /// @return Fals daca jucatorul a iesit din joc in timpul pauzei sau adevarat altfel.
    ///
    bool pauza(StadiulJocului&);
    ///
    /// @brief Printeaza scorul curent pe capul omidei.
    ///
    void pune_scor(StadiulJocului&);
    ///
    /// @brief Marcheaza totul @ref PosibilitateTeren::GOL.
    /// 
    void reseteaza_teren(StadiulJocului&);
    ///
    /// @brief Schimba @ref StadiulJocului::noua_directie_a_omizii in functie de tastele apasate.
    ///
    /// @return Fals daca jocul nu mai trebuie sa continue.
    ///
    bool verifica_taste_apasate(StadiulJocului&);

    long long m_timp_powerup = 0LL;

public:
    virtual void iesire(StadiulJocului&) override;
    virtual void incarca(StadiulJocului&) override;
    virtual bool incarcat_deja() override;
    virtual bool itereaza(StadiulJocului&) override;

    Joc() = default;
    virtual ~Joc() noexcept = default;
};

/// 
/// @brief Defineste meniul afisat la deschiderea jocului.
///
class MeniuStart : public Scena
{
    bool m_incarcat = false;

    enum tip_meniu
    {
        START_JOC = 0,
        IESIRE,
        NR_MENIURI
    };

    tip_meniu m_meniu_selectat = tip_meniu::START_JOC;

    SDL_Rect m_dreptunghi_meniu[NR_MENIURI];
    SDL_Texture* m_textura_meniu[NR_MENIURI];
    TTF_Font* m_font = nullptr;

    const char* m_meniu_text[NR_MENIURI];

    const SDL_Color m_culoare_background_meniu = { 153, 51, 153, 255 };

    bool click_in_meniu();
    bool verifica_evenimente(StadiulJocului&);

public:
    virtual void iesire(StadiulJocului&) override;
    virtual void incarca(StadiulJocului&) override;
    virtual bool incarcat_deja() override;
    virtual bool itereaza(StadiulJocului&) override;

    MeniuStart() noexcept = default;
    virtual ~MeniuStart() noexcept = default;
};

///
/// @brief Meniul care va fi afisat dupa terminarea unui joc.
///
class MeniuFinal : public Scena
{
    bool m_incarcat_deja = false;

    TTF_Font* m_font = nullptr;
    SDL_Texture* m_text = nullptr;
    SDL_Texture* m_text_scor_maxim = nullptr;

    ///
    /// @brief Fisierul in care sunt stocate toate scorurile.
    ///
    /// Istoricul are forma:
    ///     Nume Jucator
    ///     Luna zi An
    ///     Scor
    ///
    std::ifstream m_istoric{};

    int m_scor_maxim{ 0 };
    std::string m_numele_scorului_maxim{ "" };
    std::string m_anul_scorului_maxim{ "" };
    std::string m_luna_scorului_maxim{ "" };
    std::string m_ziua_scorului_maxim{ "" };

    bool m_are_scor_maxim{ false };

    std::string m_nume_curent{ "NUME" };
    SDL_Texture* m_text_nume_curent{ nullptr };

    void citeste_istoric();

public:
    virtual void iesire(StadiulJocului&) override;
    virtual void incarca(StadiulJocului&) override;
    virtual bool incarcat_deja() override;
    virtual bool itereaza(StadiulJocului&) override;

    MeniuFinal() = default;
    virtual ~MeniuFinal() noexcept = default;
};

///
/// @brief Verifica daca jucatorul a apasat tasta data.
///
bool a_apasat(SDL_Scancode tasta);
///
/// @brief Deseneaza capul omizii la pozitia data.
/// 
/// @param[in] x Coordonata lungime ca in plan cartezian.
/// @param[in] y Inaltimea, coordonata (0, 0) este in coltul din stanga sus al ferestrei.
///
/// @attention Pozitia data nu are aceleasi coordonate ca @ref Punct.
///            x si y se refera la pozitia in fereastra, nu in matrice.
///            Se aplica si la @ref deseneaza_corp, @ref deseneaza_textura.
///
void deseneaza_cap(StadiulJocului&, int x, int y);
///
/// @brief Deseneaza un corpul la pozitia data.
///
void deseneaza_corp(StadiulJocului&, int, int);
///
/// @brief Deseneaza frunza.
///
void deseneaza_frunza(StadiulJocului&);
///
/// @brief Deseneaza intreaga omida.
///
/// Parcurge @ref StadiulJocului::pozitii_omida si deseneaza fiecare parte
/// a omizii.
///
void deseneaza_omida(StadiulJocului&);
///
/// @brief Deseneaza powerup-ul(un mar) pe ecran.
///
void deseneaza_powerup(StadiulJocului&);
///
/// @brief Pune textura data la pozitia data.
///
void deseneaza_textura(SDL_Texture* textura, int, int);
///
/// Returneaza un punct la nimereala cu conditia ca in acea pozitie in matricesa nu fie nimic.
///
Punct genereaza_pozitie_noua(StadiulJocului& stadiu);
///
/// Creeaza fereastra si desenatorul.
///
void initializeaza();
///
/// Pune in matricea jocului pe pozitia @ref StadiulJocului::pozitie_frunza 
/// @ref PosibilitateTeren::FRUNZA.
///
void marcheaza_frunza(StadiulJocului&);
///
/// @brief Vede ce taste a apasat jucatorul si orice alte evenimente.
///
void verifica_evenimente();

///
/// Toata magia se intampla aici.
///
int main()
{
    initializeaza();

    global::scene.emplace_back(new Joc);
    global::scene.emplace_back(new MeniuStart);

    StadiulJocului stadiu;

    while(!global::scene.empty()) {
        if(!global::scene.back()->incarcat_deja()) {
            global::scene.back()->incarca(stadiu);
        }

        bool inca_ruleaza = true;
        long long timp_precedent = SDL_GetTicks();

        while(inca_ruleaza && !global::fereastra_inchisa) {
            stadiu.timp_trecut = SDL_GetTicks() - timp_precedent;
            timp_precedent = SDL_GetTicks();

            SDL_SetRenderDrawColor(
                    global::desenator, 
                    stadiu.culoare_fundal.r, 
                    stadiu.culoare_fundal.g,
                    stadiu.culoare_fundal.b, 
                    stadiu.culoare_fundal.a
                );
            SDL_RenderClear(global::desenator);

            global::taste_apasate.clear();
            verifica_evenimente();
            inca_ruleaza = global::scene.back()->itereaza(stadiu);
            SDL_RenderPresent(global::desenator);
        }

        if(global::scene.back()->incarcat_deja()) {
            global::scene.back()->iesire(stadiu);
            global::scene.pop_back();
        }
    }

    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}

bool a_apasat(SDL_Scancode tasta)
{ return global::taste_apasate.find(tasta) != global::taste_apasate.end(); }

void deseneaza_textura(SDL_Texture* textura, int x, int y)
{
    SDL_Rect sursa;
    SDL_Rect destinatie;

    sursa.x = 0;
    sursa.y = 0;

    SDL_QueryTexture(textura, nullptr, nullptr, &sursa.w, &sursa.h);

    destinatie.x = x;
    destinatie.y = y;
    destinatie.w = StadiulJocului::lungime_textura;
    destinatie.h = StadiulJocului::inaltime_textura;

    SDL_RenderCopy(global::desenator, textura, &sursa, &destinatie);
}

void deseneaza_cap(StadiulJocului& stadiu, int x, int y)
{ deseneaza_textura(stadiu.textura_capului, x, y); }

void deseneaza_corp(StadiulJocului& stadiu, int x, int y)
{ deseneaza_textura(stadiu.textura_corpului, x, y); }


void deseneaza_frunza(StadiulJocului& stadiu)
{
    deseneaza_textura(
        stadiu.textura_frunzei,
        stadiu.pozitie_frunza.j * StadiulJocului::lungime_textura,
        stadiu.pozitie_frunza.i * StadiulJocului::inaltime_textura
    );
}

void deseneaza_omida(StadiulJocului& stadiu)
{
    Punct pozitie_cap = stadiu.pozitii_omida.front();

    deseneaza_cap(
        stadiu,
        pozitie_cap.j * StadiulJocului::lungime_textura,
        pozitie_cap.i * StadiulJocului::inaltime_textura
    );

    auto iterator = stadiu.pozitii_omida.begin();
    ++iterator;

    while(iterator != stadiu.pozitii_omida.end()) {
        deseneaza_corp(
            stadiu,
            (*iterator).j * StadiulJocului::lungime_textura,
            (*iterator).i * StadiulJocului::inaltime_textura
        );
        ++iterator;
    }
}

void deseneaza_powerup(StadiulJocului& stadiu) {
    //SDL_Rect dest = 
    //        { stadiu.pozitie_powerup.j * StadiulJocului::lungime_textura,
    //          stadiu.pozitie_powerup.i * StadiulJocului::inaltime_textura,
    //          StadiulJocului::lungime_textura,
    //          StadiulJocului::inaltime_textura };

    //SDL_SetRenderDrawColor(global::desenator, 0, 255, 0, 255);
    //SDL_RenderFillRect(global::desenator, &dest);
    deseneaza_textura(
            stadiu.textura_powerup,
            stadiu.pozitie_powerup.j * StadiulJocului::lungime_textura,
            stadiu.pozitie_powerup.i * StadiulJocului::inaltime_textura
    );
}

Punct genereaza_pozitie_noua(StadiulJocului& stadiu)
{
    std::mt19937 generator_random;
    generator_random.seed(std::random_device{}());
    std::uniform_int_distribution<int> interval_inaltime{ 0, stadiu.inaltime_teren - 1 };
    std::uniform_int_distribution<int> interval_lungime{ 0, stadiu.lungime_teren - 1 };

    Punct pozitie = { 
        interval_inaltime(generator_random), 
        interval_lungime(generator_random) 
    };

    while(stadiu.teren[pozitie.i][pozitie.j] != PosibilitateTeren::GOL) { 
        pozitie.i = interval_inaltime(generator_random);
        pozitie.j = interval_lungime(generator_random);
    }

    return pozitie;
}

void initializeaza()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Initializarea a esuat!\n";
    }

    global::fereastra = SDL_CreateWindow("Omida", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            global::lungime, global::inaltime,
            SDL_WINDOW_SHOWN);

    SDL_SetWindowResizable(global::fereastra, SDL_FALSE);

    global::desenator = SDL_CreateRenderer(
            global::fereastra, -1, SDL_RENDERER_ACCELERATED
    );

    if(!global::fereastra || !global::desenator) {
        std::cout << "Nu s-a putut crea fereastra!\n";
    }

    if(TTF_Init() < 0) {
        std::cout << "Nu s-au putut initializa fonturile!\n";
    }

    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        std::cout << "Nu s-a putut initializa sunetul!\n";
    }
}

void marcheaza_frunza(StadiulJocului& stadiu)
{ 
    stadiu.teren[stadiu.pozitie_frunza.i]
                [stadiu.pozitie_frunza.j] = PosibilitateTeren::FRUNZA; 
}

void verifica_evenimente()
{
    SDL_Event eveniment;
    
    auto una_din_sageti = [](SDL_Event const& eveniment) 
    { 
        return eveniment.key.keysym.scancode == SDL_SCANCODE_UP
            || eveniment.key.keysym.scancode == SDL_SCANCODE_DOWN
            || eveniment.key.keysym.scancode == SDL_SCANCODE_LEFT
            || eveniment.key.keysym.scancode == SDL_SCANCODE_RIGHT;
    };

    auto una_din_hjkl = [](SDL_Event const& eveniment)
    {
        return eveniment.key.keysym.scancode == SDL_SCANCODE_J
            || eveniment.key.keysym.scancode == SDL_SCANCODE_H
            || eveniment.key.keysym.scancode == SDL_SCANCODE_K
            || eveniment.key.keysym.scancode == SDL_SCANCODE_L;
    };

    while(SDL_PollEvent(&eveniment)) {
        switch(eveniment.type) {
            case SDL_QUIT:
                global::fereastra_inchisa = true;
                break;
            case SDL_KEYDOWN:
                global::taste_apasate[eveniment.key.keysym.scancode] = true;
               
                // Daca e tasta apasata repetat
                if((una_din_sageti(eveniment) || una_din_hjkl(eveniment))
                        && eveniment.key.repeat != 0) {
                    StadiulJocului::timp_asteptare_maxim = 50LL;
                }

                break;
            case SDL_KEYUP:
                StadiulJocului::timp_asteptare_maxim = 150LL;
                break;
            default:
                break;
        }
    }
}

bool Joc::muta_omida(StadiulJocului& stadiu)
{
    Punct pozitie_cap = stadiu.pozitii_omida.front();
    pozitie_cap.i += directie[stadiu.directia_omizii].i;
    pozitie_cap.j += directie[stadiu.directia_omizii].j;

    if(pozitie_cap.i < 0 || pozitie_cap.i >= StadiulJocului::inaltime_teren) {
        return false;
    }
    if(pozitie_cap.j < 0 || pozitie_cap.j >= StadiulJocului::lungime_teren) {
        return false;
    }

    PosibilitateTeren& ce_urmeaza = stadiu.teren[pozitie_cap.i][pozitie_cap.j];

    switch(ce_urmeaza) {
        case PosibilitateTeren::OMIDA: {
            return false;
        }
        case PosibilitateTeren::FRUNZA: {
            Mix_PlayChannel(-1, m_sunet_frunza, 0);
            ++stadiu.scor;

            stadiu.pozitie_frunza = genereaza_pozitie_noua(stadiu);
            marcheaza_frunza(stadiu);

            ce_urmeaza = PosibilitateTeren::OMIDA;
            stadiu.pozitii_omida.push_front(pozitie_cap);
            return true; 
        }
        case PosibilitateTeren::POWERUP: {
            Mix_PlayChannel(-1, m_sunet_powerup, 0);
            stadiu.scor += 10;

            ce_urmeaza = PosibilitateTeren::OMIDA;
            stadiu.pozitii_omida.push_front(pozitie_cap);
            stadiu.exista_powerup = false;

            break;
        }
        case PosibilitateTeren::GOL: {
            ce_urmeaza = PosibilitateTeren::OMIDA;
            stadiu.pozitii_omida.push_front(pozitie_cap);
           
            const Punct coada = stadiu.pozitii_omida.back();
            stadiu.teren[coada.i][coada.j] = PosibilitateTeren::GOL;
            stadiu.pozitii_omida.pop_back();
            
            return true;
        }
    }

    return true;
}

bool Joc::pauza(StadiulJocului&) 
{
    bool continua = true;

    do {
        global::taste_apasate.clear();
        verifica_evenimente();

        if(a_apasat(SDL_SCANCODE_X)) {
            continua = false;
        }

    } while(!a_apasat(SDL_SCANCODE_ESCAPE) && continua);

    return continua;
}

void Joc::pune_scor(StadiulJocului& stadiu)
{
    const Punct pozitie_cap = stadiu.pozitii_omida.front();
  
    std::string scor = std::to_string(stadiu.scor);

    if(stadiu.scor < 10) {
        scor.insert(scor.begin(), ' ');
        scor += ' ';
    }

    SDL_Surface* text = TTF_RenderText_Solid(m_font, scor.c_str(), { 255, 255, 255, 255 });
    SDL_Texture* textura_scor = SDL_CreateTextureFromSurface(global::desenator, text);

    deseneaza_textura(
            textura_scor, 
            pozitie_cap.j * StadiulJocului::lungime_textura, 
            pozitie_cap.i * StadiulJocului::inaltime_textura
        ); 

    SDL_FreeSurface(text);
    SDL_DestroyTexture(textura_scor);
}

void Joc::reseteaza_teren(StadiulJocului& stadiu)
{
    for(int i = 0; i < StadiulJocului::inaltime_teren; ++i) {
        for(int j = 0; j < StadiulJocului::lungime_teren; ++j) {
            stadiu.teren[i][j] = PosibilitateTeren::GOL;
        }
    }
}

bool Joc::verifica_taste_apasate(StadiulJocului& stadiu)
{
    bool continua = true;

    if((a_apasat(SDL_SCANCODE_UP) || a_apasat(SDL_SCANCODE_K))
            && stadiu.directia_omizii != TipDirectie::JOS) 
    {
        noua_directie_a_omizii = TipDirectie::SUS;
    }
    else if((a_apasat(SDL_SCANCODE_DOWN) || a_apasat(SDL_SCANCODE_J))
            && stadiu.directia_omizii != TipDirectie::SUS) 
    {
        noua_directie_a_omizii = TipDirectie::JOS;
    }
    else if((a_apasat(SDL_SCANCODE_LEFT) || a_apasat(SDL_SCANCODE_H))
            && stadiu.directia_omizii != TipDirectie::DREAPTA) 
    {
        noua_directie_a_omizii = TipDirectie::STANGA;
    }
    else if((a_apasat(SDL_SCANCODE_RIGHT) || a_apasat(SDL_SCANCODE_L))
            && stadiu.directia_omizii != TipDirectie::STANGA) 
    {
        noua_directie_a_omizii = TipDirectie::DREAPTA;
    }
    else if(a_apasat(SDL_SCANCODE_ESCAPE)) {
        continua = this->pauza(stadiu);
    }

    return continua;
}

void Joc::incarca(StadiulJocului& stadiu)
{
    SDL_Surface* cap = IMG_Load("./IMG/cap.png");
    SDL_Surface* corp = IMG_Load("./IMG/corp.png");
    SDL_Surface* frunza = IMG_Load("./IMG/frunza.png");
    SDL_Surface* powerup = IMG_Load("./IMG/mar.png");

    SDL_SetColorKey(cap, SDL_TRUE, SDL_MapRGB(cap->format, 0, 0, 0));
    SDL_SetColorKey(corp, SDL_TRUE, SDL_MapRGB(corp->format, 0, 0, 0));
    SDL_SetColorKey(frunza, SDL_TRUE, SDL_MapRGB(frunza->format, 0, 0, 0));
    SDL_SetColorKey(powerup, SDL_TRUE, SDL_MapRGB(powerup->format, 0, 0, 0));

    stadiu.textura_capului = SDL_CreateTextureFromSurface(global::desenator, cap);
    stadiu.textura_corpului = SDL_CreateTextureFromSurface(global::desenator, corp);
    stadiu.textura_frunzei = SDL_CreateTextureFromSurface(global::desenator, frunza);
    stadiu.textura_powerup = SDL_CreateTextureFromSurface(global::desenator, powerup);
    
    if(!stadiu.textura_capului 
            || !stadiu.textura_corpului
            || !stadiu.textura_frunzei
            || !stadiu.textura_powerup) 
    { 
        std::cout << "Nu s-au putut incarca texturile!\n"; 
    }
    
    reseteaza_teren(stadiu);
    stadiu.directia_omizii = TipDirectie::SUS;
    stadiu.pozitii_omida.clear();
    stadiu.scor = 1;

    stadiu.pozitii_omida.push_back(
        { StadiulJocului::inaltime_teren / 2 - 1, StadiulJocului::lungime_teren / 2 - 1 }
    );
    stadiu.pozitii_omida.push_back(
        { StadiulJocului::inaltime_teren / 2, StadiulJocului::lungime_teren / 2 - 1 }
    );
    stadiu.pozitii_omida.push_back(
        { StadiulJocului::inaltime_teren / 2 + 1, StadiulJocului::lungime_teren / 2 - 1 }
    );

    for(Punct p : stadiu.pozitii_omida) {
        stadiu.teren[p.i][p.j] = PosibilitateTeren::OMIDA;
    }

    //stadiu.scor = stadiu.pozitii_omida.size();

    stadiu.pozitie_frunza = genereaza_pozitie_noua(stadiu);
    marcheaza_frunza(stadiu);

    m_font = TTF_OpenFont("./font/hinted-ElaineSans-Regular.ttf", 56);

    if(!m_font) {
        std::cout << "Nu s-a putut incarca fontul!\n";
    }

    m_sunet_frunza = Mix_LoadWAV("./audio/frunza.wav");
    m_sunet_powerup = Mix_LoadWAV("./audio/powerup.wav");

    if(!m_sunet_frunza) {
        std::cout << "Nu s-a putut incarca sunetul!\n";
    }

    SDL_FreeSurface(cap);
    SDL_FreeSurface(corp);
    SDL_FreeSurface(frunza);

    m_incarcat_deja = true;
}

bool Joc::incarcat_deja()
{ return m_incarcat_deja; }

bool Joc::itereaza(StadiulJocului& stadiu) 
{
    bool continua = true;

    continua = verifica_taste_apasate(stadiu);

    m_timp_asteptat += stadiu.timp_trecut;
    m_timp_powerup += stadiu.timp_trecut;

    if(m_timp_asteptat >= StadiulJocului::timp_asteptare_maxim) {
        stadiu.directia_omizii = noua_directie_a_omizii;
        continua = muta_omida(stadiu);
        m_timp_asteptat = 0LL;
    }
    if(m_timp_powerup >= 20000LL) {
        if(!stadiu.exista_powerup) {
            stadiu.pozitie_powerup = genereaza_pozitie_noua(stadiu);
            stadiu.teren
                [stadiu.pozitie_powerup.i]
                [stadiu.pozitie_powerup.j] = PosibilitateTeren::POWERUP;
            stadiu.exista_powerup = true;
        }

        m_timp_powerup = 0LL;
    }

    deseneaza_omida(stadiu);
    deseneaza_frunza(stadiu);
    pune_scor(stadiu);

    if(stadiu.exista_powerup) {
        deseneaza_powerup(stadiu);
    }

    return continua;
}

void Joc::iesire(StadiulJocului&)
{
    Mix_FreeChunk(m_sunet_frunza);

    TTF_CloseFont(m_font);

    global::scene.emplace_front(new MeniuFinal);
}

bool MeniuStart::click_in_meniu()
{
    auto punct_in_dreptunghi = 
    [](int const x, int const y, SDL_Rect const& dreptunghi) -> bool
    {
        if(x < dreptunghi.x || y < dreptunghi.y) {
            return false;
        }
        if(x > dreptunghi.x + dreptunghi.w || y > dreptunghi.y + dreptunghi.h) {
            return false;
        }

        return true;
    };

    int x, y;

    bool click_stanga = SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT);

    for(int i = 0; i < NR_MENIURI; ++i) {
        if(click_stanga && punct_in_dreptunghi(x, y, m_dreptunghi_meniu[i])) {
            m_meniu_selectat = (tip_meniu)i;
            return true;
        }
    }

    return false;
}

bool MeniuStart::verifica_evenimente(StadiulJocului&)
{
    if(a_apasat(SDL_SCANCODE_ESCAPE) || a_apasat(SDL_SCANCODE_X)) {
        global::fereastra_inchisa = true;
        return false;
    }
    if(a_apasat(SDL_SCANCODE_P)) {
        return false;
    }

    if(a_apasat(SDL_SCANCODE_UP) || a_apasat(SDL_SCANCODE_K)) {
        m_meniu_selectat = tip_meniu::START_JOC;
    }
    if(a_apasat(SDL_SCANCODE_DOWN) || a_apasat(SDL_SCANCODE_J)) {
        m_meniu_selectat = tip_meniu::IESIRE;
    }

    return true;
}

void MeniuStart::iesire(StadiulJocului&)
{
    for(int i = 0; i < NR_MENIURI; ++i) {
        SDL_DestroyTexture(m_textura_meniu[i]);
    }

    TTF_CloseFont(m_font);
}

void MeniuStart::incarca(StadiulJocului&)// stadiu)
{
    m_meniu_text[0] = "JOACA";
    m_meniu_text[1] = "IESIRE";

    m_font = TTF_OpenFont("./font/hinted-ElaineSans-Bold.ttf", 80);

    for(int i = 0; i < NR_MENIURI; ++i) {
        m_dreptunghi_meniu[i] = {
            global::lungime / 2 - global::lungime / 6,
            (global::inaltime / 4) * (i + 1),
            2 * global::lungime / 6,
            global::inaltime / 5
        };

        SDL_Surface* text = TTF_RenderText_Blended(
                m_font, m_meniu_text[i], 
                { 255, 255, 255, 255 }
                //stadiu.culoare_fundal
        );

        m_textura_meniu[i] = SDL_CreateTextureFromSurface(
                global::desenator, text
        );

        SDL_FreeSurface(text);
    }

    m_incarcat = true; 
}

bool MeniuStart::incarcat_deja()
{ return m_incarcat; }

bool MeniuStart::itereaza(StadiulJocului& stadiu)
{
    bool continua = true;

    if(a_apasat(SDL_SCANCODE_ESCAPE) || a_apasat(SDL_SCANCODE_X)) {
        global::fereastra_inchisa = true;
        continua = false;
    }
    if(a_apasat(SDL_SCANCODE_P)) {
        continua = false;
    }

    continua = this->verifica_evenimente(stadiu);

    if(a_apasat(SDL_SCANCODE_RETURN) || click_in_meniu()) {
        switch(m_meniu_selectat) {
            case START_JOC: 
                continua = false; break;
            case IESIRE:    
                continua = false; 
                global::fereastra_inchisa = true;
                break;
            default: break;
        }
    }

    SDL_SetRenderDrawColor(
            global::desenator, 
            m_culoare_background_meniu.r,
            m_culoare_background_meniu.g,
            m_culoare_background_meniu.b,
            m_culoare_background_meniu.a
    );

    SDL_RenderFillRect(
            global::desenator, 
            &m_dreptunghi_meniu[m_meniu_selectat]
    );

    for(int i = 0; i < NR_MENIURI; ++i) {
        SDL_SetRenderDrawColor(
                global::desenator,
                255, 255, 255, 255
        );

        SDL_RenderDrawRect(
                global::desenator, 
                &m_dreptunghi_meniu[i]
        );

        SDL_Rect dreptunghi = {
            m_dreptunghi_meniu[i].x + 5 * stadiu.lungime_textura / 10,
            m_dreptunghi_meniu[i].y + 5 * stadiu.inaltime_textura / 10,
            m_dreptunghi_meniu[i].w - 2 * 5 * stadiu.lungime_textura / 10,
            m_dreptunghi_meniu[i].h - 2 * 5 * stadiu.inaltime_textura / 10
        };

        SDL_RenderCopy(
                global::desenator, 
                m_textura_meniu[i], 
                nullptr, 
                &dreptunghi
                //&m_dreptunghi_meniu[i]
        );
    }

    return continua;
}

void MeniuFinal::citeste_istoric()
{
    std::getline(m_istoric, m_numele_scorului_maxim);
    m_istoric >> m_ziua_scorului_maxim;
    m_istoric >> m_luna_scorului_maxim;
    m_istoric >> m_anul_scorului_maxim;
    m_istoric >> m_scor_maxim;
}

void MeniuFinal::iesire(StadiulJocului& stadiu)
{
    TTF_CloseFont(m_font);
    SDL_DestroyTexture(m_text);
    SDL_DestroyTexture(m_text_nume_curent);

    if(m_are_scor_maxim) {
        std::ofstream g{ "istoric.txt" };
        g << m_nume_curent << '\n';
        g << __DATE__ << '\n';
        g << stadiu.scor << '\n';
    }

    try {
        m_istoric.close();
    }
    catch(...) {
    }
}

void MeniuFinal::incarca(StadiulJocului& stadiu)
{
    m_font = TTF_OpenFont("./font/hinted-ElaineSans-Medium.ttf", 70);

    std::string mesaj = " Scor: ";
    mesaj += std::to_string(stadiu.scor);

    SDL_Surface* text = TTF_RenderText_Blended_Wrapped(
            m_font,
            mesaj.c_str(),
            { 255, 255, 255, 255 },
            600
    );

    m_text = SDL_CreateTextureFromSurface(global::desenator, text);
    SDL_FreeSurface(text);

    m_istoric.open("istoric.txt");
    this->citeste_istoric();

    if(stadiu.scor < m_scor_maxim) {
        std::string scor_maxim = "Scor maxim: ";
        scor_maxim += std::to_string(m_scor_maxim);

        if(m_numele_scorului_maxim != "Start") {
            scor_maxim += "(";
            scor_maxim += m_numele_scorului_maxim;
            scor_maxim += ")";
        }

        text = TTF_RenderText_Blended(
                m_font,
                scor_maxim.c_str(),
                { 255, 255, 255, 255 }
        );

        m_text_scor_maxim = SDL_CreateTextureFromSurface(
                global::desenator, text
        );
    }
    else if(stadiu.scor >= m_scor_maxim) {
        m_are_scor_maxim = true;
        std::ofstream g{ "istoric.txt" };

        std::string scor_maxim = "    Nou maxim!    ";

        text = TTF_RenderText_Blended(
                m_font,
                scor_maxim.c_str(),
                { 255, 255, 255, 255 }
        );

        m_text_scor_maxim = SDL_CreateTextureFromSurface(
                global::desenator, text
        );
    }

    SDL_FreeSurface(text);

    m_incarcat_deja = true;
}

bool MeniuFinal::incarcat_deja()
{ return m_incarcat_deja; }

bool e_litera(SDL_Scancode t_tasta)
{
    return t_tasta == SDL_SCANCODE_A || t_tasta == SDL_SCANCODE_B
        || t_tasta == SDL_SCANCODE_B || t_tasta == SDL_SCANCODE_C
        || t_tasta == SDL_SCANCODE_D || t_tasta == SDL_SCANCODE_E
        || t_tasta == SDL_SCANCODE_F || t_tasta == SDL_SCANCODE_G
        || t_tasta == SDL_SCANCODE_H || t_tasta == SDL_SCANCODE_I
        || t_tasta == SDL_SCANCODE_J || t_tasta == SDL_SCANCODE_K
        || t_tasta == SDL_SCANCODE_L || t_tasta == SDL_SCANCODE_M
        || t_tasta == SDL_SCANCODE_N || t_tasta == SDL_SCANCODE_O
        || t_tasta == SDL_SCANCODE_P || t_tasta == SDL_SCANCODE_Q
        || t_tasta == SDL_SCANCODE_R || t_tasta == SDL_SCANCODE_S
        || t_tasta == SDL_SCANCODE_T || t_tasta == SDL_SCANCODE_U
        || t_tasta == SDL_SCANCODE_V || t_tasta == SDL_SCANCODE_W
        || t_tasta == SDL_SCANCODE_X || t_tasta == SDL_SCANCODE_Y
        || t_tasta == SDL_SCANCODE_Z;
}

bool e_numar(SDL_Scancode t_tasta)
{
    return t_tasta == SDL_SCANCODE_1 || t_tasta == SDL_SCANCODE_2
        || t_tasta == SDL_SCANCODE_3 || t_tasta == SDL_SCANCODE_4
        || t_tasta == SDL_SCANCODE_5 || t_tasta == SDL_SCANCODE_6
        || t_tasta == SDL_SCANCODE_7 || t_tasta == SDL_SCANCODE_8
        || t_tasta == SDL_SCANCODE_9 || t_tasta == SDL_SCANCODE_0;
}

bool MeniuFinal::itereaza(StadiulJocului&)
{
    bool continua = true;

    if(a_apasat(SDL_SCANCODE_RETURN)) {
        global::scene.emplace_front(new MeniuStart);
        global::scene.emplace_front(new Joc);
        continua = false;
    }
    if(a_apasat(SDL_SCANCODE_ESCAPE)) {
        continua = false;
    }
    if(a_apasat(SDL_SCANCODE_BACKSPACE) && !m_nume_curent.empty()) {
        m_nume_curent.pop_back();
    }
    if(a_apasat(SDL_SCANCODE_SPACE)) {
        m_nume_curent += " ";
    }

    for(auto const& tasta : global::taste_apasate) {
        if(e_litera(tasta.first) || e_numar(tasta.first)) {
            m_nume_curent += SDL_GetKeyName(
                    SDL_GetKeyFromScancode(tasta.first)
            );
        }
    }

    SDL_Rect destinatie;
    destinatie.x = global::lungime / 3;
    destinatie.y = global::inaltime / 3 - global::inaltime / 16;
    destinatie.w = global::lungime / 3;
    destinatie.h = global::inaltime / 8;

    SDL_Rect destinatie_text_scor;
    destinatie_text_scor.x = global::lungime / 8;
    destinatie_text_scor.y = global::inaltime / 3 + global::inaltime / 16;
    destinatie_text_scor.w = global::lungime / 1.3f;
    destinatie_text_scor.h = global::inaltime / 8;

    SDL_RenderCopy(global::desenator, m_text, nullptr, &destinatie);
    SDL_RenderCopy(
            global::desenator,
            m_text_scor_maxim,
            nullptr,
            &destinatie_text_scor
    );

    if(m_are_scor_maxim) {
        SDL_Rect prompt;
        prompt.x = global::lungime / 4;
        prompt.y = global::inaltime / 3 + 4 * global::inaltime / 16;
        prompt.w = global::lungime / 2;
        prompt.h = global::inaltime / 8;

        SDL_SetRenderDrawColor(global::desenator, 0, 0, 0, 0);
        SDL_RenderFillRect(global::desenator, &prompt);

        std::size_t const lungime_max = 20;
        std::size_t i{ 0 };

        std::string nume = "";
        nume.reserve(lungime_max + 1);

        for(i = 0; i < lungime_max - m_nume_curent.size() / 2; ++i) {
            nume.push_back(' ');
        }
        nume += m_nume_curent;
        for(i = 0; i < lungime_max - m_nume_curent.size() / 2; ++i) {
            nume.push_back(' ');
        }

        SDL_Surface* text = TTF_RenderText_Blended(
                m_font, nume.c_str(), { 255, 255, 255, 255 }
        );

        m_text_nume_curent = SDL_CreateTextureFromSurface(
                global::desenator, text
        );

        SDL_RenderCopy(global::desenator, m_text_nume_curent, nullptr, &prompt);

        SDL_FreeSurface(text);
    }

    return continua;
}

