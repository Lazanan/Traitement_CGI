// /var/www/mywebsite/cgi-bin/login_handler.cgi
#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

// Fonction pour décoder les caractères URL encodés
std::string decoderURL(const std::string& str) {
    std::string decode;
    char ch;
    int i, j;
    for (i = 0; i < str.length(); i++) {
        if (str[i] == '%') {
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &j);
            ch = static_cast<char>(j);
            decode += ch;
            i += 2;
        } else if (str[i] == '+') {
            decode += ' ';
        } else {
            decode += str[i];
        }
    }
    return decode;
}

// Fonction pour vérifier les informations d'identification dans un fichier
bool verifierIdentifiants(const std::string& utilisateur, const std::string& motDePasse) {
    std::ifstream fichier("/var/www/mywebsite/data.txt");
    std::string ligne;
    while (std::getline(fichier, ligne)) {
        std::string::size_type pos = ligne.find(':');
        if (pos != std::string::npos) {
            std::string utilisateurFichier = ligne.substr(0, pos);
            std::string motDePasseFichier = ligne.substr(pos + 1);
            if (utilisateurFichier == utilisateur && motDePasseFichier == motDePasse) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    // En-têtes HTTP pour indiquer que le contenu est en HTML
    std::cout << "Content-Type: text/html\n\n";

    // Lire la chaîne de requête GET à partir des variables d'environnement
    std::string chaineRequete = std::getenv("QUERY_STRING");
    
    // Analyser les paramètres de la requête GET
    std::map<std::string, std::string> parametresRequete;
    std::istringstream flux(chaineRequete);
    std::string paire;
    while (std::getline(flux, paire, '&')) {
        std::string::size_type pos = paire.find('=');
        if (pos != std::string::npos) {
            std::string cle = decoderURL(paire.substr(0, pos));
            std::string valeur = decoderURL(paire.substr(pos + 1));
            parametresRequete[cle] = valeur;
        }
    }

    // Récupérer les valeurs du formulaire de connexion
    std::string utilisateur = parametresRequete["username"];
    std::string motDePasse = parametresRequete["password"];

    // Vérifier les informations d'identification fournies
    if (verifierIdentifiants(utilisateur, motDePasse)) {
        std::cout << "<html><body><h1>Bienvenue, " << utilisateur << "!</h1></body></html>";
    } else {
        std::cout << "<html><body>";
        std::cout << "<h1>Nom d'utilisateur ou mot de passe invalide.</h1>";
        std::cout << "<p><a href=\"/form.html\">Retour au formulaire de connexion</a></p>";
        std::cout << "</body></html>";
    }

    return 0;
}
