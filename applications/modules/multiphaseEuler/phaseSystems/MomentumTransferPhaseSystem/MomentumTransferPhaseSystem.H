/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2015-2025 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::MomentumTransferPhaseSystem

Description
    Class which models interfacial momentum transfer between a number of phases.
    Drag, virtual mass, lift, wall lubrication and turbulent dispersion are all
    modelled. The explicit contribution from the drag is omitted from the
    transfer matrices, as this forms part of the solution of the pressure
    equation.

SourceFiles
    MomentumTransferPhaseSystem.C

\*---------------------------------------------------------------------------*/

#ifndef MomentumTransferPhaseSystem_H
#define MomentumTransferPhaseSystem_H

#include "phaseSystem.H"
#include "HashPtrTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class blendingMethod;
class blendedDragModel;
class blendedVirtualMassModel;
class blendedLiftModel;
class blendedWallLubricationModel;
class blendedTurbulentDispersionModel;

/*---------------------------------------------------------------------------*\
                 Class MomentumTransferPhaseSystem Declaration
\*---------------------------------------------------------------------------*/

template<class BasePhaseSystem>
class MomentumTransferPhaseSystem
:
    public BasePhaseSystem
{
    // Private typedefs

        typedef HashPtrTable
        <
            volScalarField,
            phaseInterfaceKey,
            phaseInterfaceKey::hash
        > KdTable;

        typedef HashPtrTable
        <
            blendedDragModel,
            phaseInterfaceKey,
            phaseInterfaceKey::hash
        > dragModelTable;

        typedef HashPtrTable
        <
            blendedVirtualMassModel,
            phaseInterfaceKey,
            phaseInterfaceKey::hash
        > virtualMassModelTable;

        typedef HashPtrTable
        <
            blendedLiftModel,
            phaseInterfaceKey,
            phaseInterfaceKey::hash
        > liftModelTable;

        typedef HashPtrTable
        <
            blendedWallLubricationModel,
            phaseInterfaceKey,
            phaseInterfaceKey::hash
        > wallLubricationModelTable;

        typedef HashPtrTable
        <
            blendedTurbulentDispersionModel,
            phaseInterfaceKey,
            phaseInterfaceKey::hash
        > turbulentDispersionModelTable;


    // Private Data

        //- Cached drag coefficients for dragCorrs
        mutable KdTable Kds_;


        // Sub Models

            //- Drag models
            const dragModelTable dragModels_;

            //- Virtual mass models
            const virtualMassModelTable virtualMassModels_;

            //- Lift models
            const liftModelTable liftModels_;

            //- Wall lubrication models
            const wallLubricationModelTable wallLubricationModels_;

            //- Turbulent dispersion models
            const turbulentDispersionModelTable turbulentDispersionModels_;


protected:

    // Protected Member Functions

        //- Add momentum transfer terms which result from bulk mass transfers
        void addDmdtUfs
        (
            const phaseSystem::dmdtfTable& dmdtfs,
            phaseSystem::momentumTransferTable& eqns
        );

        void addTmpField
        (
            tmp<surfaceScalarField>& result,
            const tmp<surfaceScalarField>& field
        ) const;

        //- Invert the ADVs matrix inplace
        void invADVs(List<UPtrList<scalarField>>& ADVs) const;

        //- Invert the ADVs matrix inplace
        template<class GeoMesh>
        void invADVs
        (
            PtrList<PtrList<GeometricField<scalar, GeoMesh>>>& ADVs
        ) const;


public:

    // Constructors

        //- Construct from fvMesh
        MomentumTransferPhaseSystem(const fvMesh&);


    //- Destructor
    virtual ~MomentumTransferPhaseSystem();


    // Member Functions

        //- Return the momentum transfer matrices for the cell-based algorithm.
        //  This includes implicit and explicit forces that add into the cell
        //  UEqn in the normal way.
        virtual autoPtr<phaseSystem::momentumTransferTable> momentumTransfer();

        //- As momentumTransfer, but for the face-based algorithm
        virtual autoPtr<phaseSystem::momentumTransferTable> momentumTransferf();

        //- Return the explicit force fluxes for the cell-based algorithm, that
        //  do not depend on phase mass/volume fluxes, and can therefore be
        //  evaluated outside the corrector loop. This includes things like
        //  lift, turbulent dispersion, and wall lubrication.
        virtual PtrList<surfaceScalarField> Fs() const;

        //- As Fs, but for the face-based algorithm
        virtual PtrList<surfaceScalarField> Ffs() const;

        //- Return the inverse of the central + drag + virtual mass
        //  coefficient matrix
        virtual void invADVs
        (
            const PtrList<volScalarField>& As,
            PtrList<volVectorField>& HVms,
            PtrList<PtrList<volScalarField>>& invADVs,
            PtrList<PtrList<surfaceScalarField>>& invADVfs
        ) const;

        //- Return the inverse of the central + drag + virtual mass
        //  coefficient matrix
        virtual PtrList<PtrList<surfaceScalarField>> invADVfs
        (
            const PtrList<surfaceScalarField>& Afs,
            PtrList<surfaceScalarField>& HVmfs
        ) const;

        //- Returns true if the phase pressure is treated implicitly
        //  in the phase fraction equation
        virtual bool implicitPhasePressure(const phaseModel& phase) const;

        //- Returns true if the phase pressure is treated implicitly
        //  in the phase fraction equation for any phase
        virtual bool implicitPhasePressure() const;

        //- Return the phase diffusivity
        //  divided by the momentum central coefficient
        virtual tmp<surfaceScalarField> alphaDByAf
        (
            const PtrList<volScalarField>& rAs
        ) const;

        //- Return the flux corrections for the cell-based algorithm. These
        //  depend on phase mass/volume fluxes, and must therefore be evaluated
        //  inside the corrector loop.
        virtual PtrList<surfaceScalarField> ddtCorrs() const;

        //- Set the cell and faces drag correction fields
        virtual void dragCorrs
        (
            PtrList<volVectorField>& dragCorrs,
            PtrList<surfaceScalarField>& dragCorrf
        ) const;

        //- Read base phaseProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "MomentumTransferPhaseSystem.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
