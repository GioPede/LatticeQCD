#include "Observables/superobs.h"
#include "Observables/observable.h"
#include "Math/su3.h"
#include "ParallelTools/parallel.h"
#include "Math/lattice.h"
#include <cstdio>
#include <cmath>

int leviCivit(int i, int j, int k, int l){
    return ( (i-j)*(i-k)*(i-l)*(j-k)*(j-l)*(k-l)/12 );
}

// CONSTRUCTOR
SuperObs::SuperObs() : Observable(){
    m_name = "SuperObs";
}

// INITIALIZER
void SuperObs::initObservable(Lattice* lattice){
    m_lat = lattice;
    m_size = m_lat->getSize();
}

void SuperObs::superGatherResults(){
    MPI_Allreduce(&plaq, &plaq, 1, MPI_DOUBLE, MPI_SUM, Parallel::cartCoordComm());
    MPI_Allreduce(&energy, &energy, 1, MPI_DOUBLE, MPI_SUM, Parallel::cartCoordComm());
    MPI_Allreduce(&topc, &topc, 1, MPI_DOUBLE, MPI_SUM, Parallel::cartCoordComm());
}

// COMPUTE THE VALUE OF THE PLAQUETTE ON THE WHOLE LATTICE
void SuperObs::compute(){
    // loop through the sublattice
    m_value = 0;
    plaqSum.setSU3Zero();
    topc = 0;
    energy = 0;
    for(int x = 0; x < m_size[0]; x++){
    for(int y = 0; y < m_size[1]; y++){
    for(int z = 0; z < m_size[2]; z++){
    for(int t = 0; t < m_size[3]; t++){

        for(int mu = 0; mu < 4; mu++){
        for(int nu = mu+1; nu < 4; nu++){
            // compute G_mu_nu
            clovers[mu][nu]  =  (*m_lat)(x,y,z,t)[mu]
                             *    m_lat->shift(x,y,z,t,nu, mu, 1)
                             *  ~(m_lat->shift(x,y,z,t,mu, nu, 1))
                             * ~(*m_lat)(x,y,z,t)[nu];

            plaqSum += clovers[mu][nu];

            clovers[mu][nu] +=  (*m_lat)(x,y,z,t)[nu]
                             *  ~(m_lat->shift2 (x,y,z,t,mu,nu,1,mu,-1))
                             *  ~(m_lat->shift(x,y,z,t,nu, mu, -1))
                             *   (m_lat->shift(x,y,z,t,mu, mu, -1));

            clovers[mu][nu] +=  ~(m_lat->shift(x,y,z,t,mu, mu, -1))
                             *  ~(m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,-1))
                             *   (m_lat->shift2 (x,y,z,t,mu,nu,-1,mu,-1))
                             *   (m_lat->shift(x,y,z,t,nu, nu, -1));

            clovers[mu][nu] +=  ~(m_lat->shift(x,y,z,t,nu, nu, -1))
                             *   (m_lat->shift(x,y,z,t,mu, nu, -1))
                             *   (m_lat->shift2 (x,y,z,t,nu,nu,-1,mu,1))
                             * ~(*m_lat)(x,y,z,t)[mu];

            // ????????

            clovers[mu][nu] = clovers[mu][nu] - (~clovers[mu][nu]);
            double tr = clovers[mu][nu].imagTrace()/3.0;
            for(int i = 1; i < 18; i+=8)
                clovers[mu][nu].mat[i] -= tr;


            clovers[mu][nu]*=0.25;
            //for(int i = 0; i < 18; i+=2)
            //    clovers[mu][nu].mat[i] = 0;
            clovers[nu][mu] = ~(clovers[mu][nu]);
            energy += (clovers[mu][nu]*clovers[mu][nu]).realTrace() + (clovers[nu][mu]*clovers[nu][mu]).realTrace();
        }}
        for(int mu = 0; mu < 4; mu++){
        for(int nu = mu+1; nu < 4; nu++){
            if(nu!=mu){
                for(int rho = 0; rho < 4; rho++){
                if(rho!=mu && rho!=nu){
                    for(int sig = 0; sig < 4; sig++){
                    if(sig!=mu && sig!=nu && sig!=rho){
                        topc += (clovers[mu][nu]*clovers[rho][sig]).realTrace()*leviCivit(mu, nu, rho, sig);
                    }}
                }}
            }
        }}

    }}}}
    plaq = plaqSum.realTrace()/ 18.0 / m_size[0] / m_size[1] / m_size[2] / m_size[3] / Parallel::numProcs();
    superGatherResults();
}


