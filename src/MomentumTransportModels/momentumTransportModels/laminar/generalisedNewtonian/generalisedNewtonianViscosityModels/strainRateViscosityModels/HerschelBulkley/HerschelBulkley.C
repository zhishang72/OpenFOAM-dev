/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2018-2025 OpenFOAM Foundation
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

#include "HerschelBulkley.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace laminarModels
{
namespace generalisedNewtonianViscosityModels
{
    defineTypeNameAndDebug(HerschelBulkley, 0);

    addToRunTimeSelectionTable
    (
        generalisedNewtonianViscosityModel,
        HerschelBulkley,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::laminarModels::generalisedNewtonianViscosityModels::HerschelBulkley::
HerschelBulkley
(
    const dictionary& viscosityProperties,
    const Foam::viscosity& viscosity,
    const volVectorField& U
)
:
    strainRateViscosityModel(viscosityProperties, viscosity, U),
    n_("n", dimless, 0),
    k_("k", dimKinematicViscosity*pow(dimTime, n_ - 1), 0),
    tau0_("tau0", dimKinematicViscosity/dimTime, 0)
{
    read(viscosityProperties);
    correct();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::laminarModels::generalisedNewtonianViscosityModels::
HerschelBulkley::read
(
    const dictionary& viscosityProperties
)
{
    strainRateViscosityModel::read(viscosityProperties);

    const dictionary& coeffs =
        viscosityProperties.optionalSubDict(typeName + "Coeffs");

    n_.read(coeffs);
    k_.dimensions().reset(dimKinematicViscosity*pow(dimTime, n_ - 1));
    k_.read(coeffs);
    tau0_.read(coeffs);

    return true;
}


Foam::tmp<Foam::volScalarField>
Foam::laminarModels::generalisedNewtonianViscosityModels::HerschelBulkley::
nu
(
    const volScalarField& nu0,
    const volScalarField& strainRate
) const
{
    return
    (
        min
        (
            nu0,
            (tau0_ + k_*pow(strainRate, n_))
           /max(strainRate, dimensionedScalar(dimless/dimTime, rootVSmall))
        )
    );
}


// ************************************************************************* //
