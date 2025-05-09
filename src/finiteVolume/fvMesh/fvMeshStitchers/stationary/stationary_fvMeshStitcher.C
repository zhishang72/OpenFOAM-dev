/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2021-2025 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "stationary_fvMeshStitcher.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace fvMeshStitchers
{
    defineTypeNameAndDebug(stationary, 0);
    addToRunTimeSelectionTable(fvMeshStitcher, stationary, fvMesh);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::fvMeshStitchers::stationary::conformCorrectMeshPhi
(
    surfaceScalarField& phi
)
{
    NotImplemented;
}


void Foam::fvMeshStitchers::stationary::createNonConformalCorrectMeshPhiGeometry
(
    surfaceLabelField::Boundary& polyFacesBf,
    surfaceVectorField& SfSf,
    surfaceVectorField& CfSf
)
{
    NotImplemented;
}


void Foam::fvMeshStitchers::stationary::unconformCorrectMeshPhi
(
    const surfaceLabelField::Boundary& polyFacesBf,
    surfaceVectorField& SfSf,
    surfaceVectorField& CfSf,
    surfaceScalarField& phi
)
{
    NotImplemented;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fvMeshStitchers::stationary::stationary(fvMesh& mesh)
:
    fvMeshStitcher(mesh)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fvMeshStitchers::stationary::~stationary()
{}


// ************************************************************************* //
