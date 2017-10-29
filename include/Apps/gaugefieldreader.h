#pragma once
#include <vector>
#include <random>
#include "Apps/app.h"


class GaugeFieldReader : public App {
    public:
        GaugeFieldReader(class InputParser* input, class Parallel* parallel);

        // main functions
        void initGFR();
        void sampleConfigurations();

        // setters-getters
        void setSize(std::vector<int> size);
        void addObservable(class Observable* observable);
        const char* getOutDir() { return m_outDir; }
        class Point& getLatticeSite(int x, int y, int z, int t);
        class Parallel* getParallel() { return m_parallel; }
        std::vector<int>& getSize() { return m_size; }
        std::vector<double>& getObsValues() { return m_obsValues; }
        std::vector<class Observable*>& getObs() { return m_obs; }


    private:
        // member classes
        class Lattice* m_lat = nullptr;
        class Parallel* m_parallel = nullptr;
        class InputConf* m_inputConf = nullptr;
        std::vector<class Observable*> m_obs;
        std::vector<double> m_obsValues;
        std::vector<std::string> m_inputConfList;
        std::vector<int> m_size;


        // member variables
        const char* m_outDir;
};
