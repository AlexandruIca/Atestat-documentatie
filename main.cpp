///
/// @file 
///
/// @mainpage Pagina principala.
///

#include <deque>
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
    FRUNZA
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
    constexpr int lungime = 900;
    ///
    /// @brief Inaltimea ferestrei.
    ///
    constexpr int inaltime = 900;

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
    std::vector<std::unique_ptr<Scena>> scene;
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
    /// @brief Initializeza scena.
    ///
    virtual void incarca(StadiulJocului&) = 0;
    ///
    /// @brief Verifica daca scena curenta a fost incarcata deja.
    ///
    virtual bool incarcat_deja() = 0;
    ///
    /// @brief Paraseste scena.
    ///
    virtual void iesire(StadiulJocului&) = 0;
    ///
    /// @brief Executa functia la fiecare 'desenare'.
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

    bool muta_omida(StadiulJocului&);
    ///
    /// @brief Ruleaza cat timp jocul e in pauza.
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

public:
    virtual void iesire(StadiulJocului&) override;
    virtual void incarca(StadiulJocului&) override;
    virtual bool incarcat_deja() override;
    virtual bool itereaza(StadiulJocului&) override;

    MeniuFinal() noexcept = default;
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
void deseneaza_omida(StadiulJocului&);
///
/// @brief Pune textura data la pozitia data.
///
void deseneaza_textura(SDL_Texture* textura, int, int);
///
/// @brief Returneaza un punct la nimereala cu conditia ca in acea pozitie in matricesa nu fie nimic.
///
Punct genereaza_pozitie_noua(StadiulJocului& stadiu);
///
/// @brief Creeaza fereastra si desenatorul.
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
        case PosibilitateTeren::OMIDA:
            return false;
        case PosibilitateTeren::FRUNZA:
            Mix_PlayChannel(-1, m_sunet_frunza, 0);
            ++stadiu.scor;

            stadiu.pozitie_frunza = genereaza_pozitie_noua(stadiu);
            marcheaza_frunza(stadiu);

            ce_urmeaza = PosibilitateTeren::OMIDA;
            stadiu.pozitii_omida.push_front(pozitie_cap);
            return true;
        case PosibilitateTeren::GOL:
            ce_urmeaza = PosibilitateTeren::OMIDA;
            stadiu.pozitii_omida.push_front(pozitie_cap);
           
            const Punct coada = stadiu.pozitii_omida.back();
            stadiu.teren[coada.i][coada.j] = PosibilitateTeren::GOL;
            stadiu.pozitii_omida.pop_back();
            
            return true;
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

    SDL_SetColorKey(cap, SDL_TRUE, SDL_MapRGB(cap->format, 0, 0, 0));
    SDL_SetColorKey(corp, SDL_TRUE, SDL_MapRGB(cap->format, 0, 0, 0));
    SDL_SetColorKey(frunza, SDL_TRUE, SDL_MapRGB(cap->format, 0, 0, 0));

    stadiu.textura_capului = SDL_CreateTextureFromSurface(global::desenator, cap);
    stadiu.textura_corpului = SDL_CreateTextureFromSurface(global::desenator, corp);
    stadiu.textura_frunzei = SDL_CreateTextureFromSurface(global::desenator, frunza);
    
    if(!stadiu.textura_capului 
            || !stadiu.textura_corpului
            || !stadiu.textura_frunzei) 
    { 
        std::cout << "Nu s-au putut incarca texturile!\n"; 
    }
    
    reseteaza_teren(stadiu);

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

    if(m_timp_asteptat >= StadiulJocului::timp_asteptare_maxim) {
        stadiu.directia_omizii = noua_directie_a_omizii;
        continua = muta_omida(stadiu);
        m_timp_asteptat = 0LL;
    }

    deseneaza_omida(stadiu);
    deseneaza_frunza(stadiu);
    pune_scor(stadiu);

    return continua;
}

void Joc::iesire(StadiulJocului& stadiu) 
{ 
    std::cout << "Scor: " << stadiu.scor << '\n';

    Mix_FreeChunk(m_sunet_frunza);

    TTF_CloseFont(m_font);
    auto a = global::desenator;
    (void)a;
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

void MeniuFinal::iesire(StadiulJocului&)
{}

void MeniuFinal::incarca(StadiulJocului&)
{
    m_incarcat_deja = true;
}

bool MeniuFinal::incarcat_deja()
{ return m_incarcat_deja; }

bool MeniuFinal::itereaza(StadiulJocului&)
{
    bool continua = true;

    return continua;
}

