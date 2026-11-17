#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

template <typename T>
class BinaryTree{
    private:
    int _depth;
    std::vector<std::vector<T>> _tree;  // _tree[n][i] : valeur au pas n, noeud i (0 <= i <= n)

    public:
    BinaryTree(): _depth(0), _tree(1, std::vector<T>(1)){}

    void setDepth(int N){
        if (N < 0){
            throw std::invalid_argument("the depth should be nonnegative");
        }
        _depth = N;
        _tree.resize(N + 1);        // supprime ou ajoute des niveaux
        for (int n = 0; n <= N; n++){
            _tree[n].resize(n + 1); // n+1 noeuds au pas n
        }
    }

    void setNode(int n, int i, T value){
        _tree.at(n).at(i) = value;
    }

    T getNode(int n, int i) const{
        return _tree.at(n).at(i);
    }

    void display() const{
        // 1) les valeurs, un pas de temps par ligne
        for (int n = 0; n <= _depth; n++){
            for (int i = 0; i <= n; i++){
                std::cout << _tree[n][i] << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // 2) le même contenu dessiné sous forme d'arbre
        std::vector<std::vector<std::string>> labels(_depth + 1);
        int width = 1;
        for (int n = 0; n <= _depth; n++){
            for (int i = 0; i <= n; i++){
                std::ostringstream os;
                os << _tree[n][i];
                labels[n].push_back(os.str());
                if ((int)os.str().size() > width){
                    width = (int)os.str().size();
                }
            }
        }

        int cell = width + 1;
        int lineLength = (2 * _depth + 1) * cell + 1;
        for (int n = 0; n <= _depth; n++){
            std::string values(lineLength, ' ');
            std::string links(lineLength, ' ');
            for (int i = 0; i <= n; i++){
                // le noeud (n, i) est centré sur la colonne de position _depth - n + 2i
                int center = (_depth - n + 2 * i) * cell + cell / 2;
                const std::string& s = labels[n][i];
                values.replace(center - (int)s.size() / 2, s.size(), s);
                links[center - cell / 2] = '/';
                links[center + cell / 2] = '\\';
            }
            std::cout << values.substr(0, values.find_last_not_of(' ') + 1) << std::endl;
            if (n < _depth){
                std::cout << links.substr(0, links.find_last_not_of(' ') + 1) << std::endl;
            }
        }
        std::cout << std::endl;
    }
};
