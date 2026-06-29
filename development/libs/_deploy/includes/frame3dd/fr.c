#include "fr.h"
double length(nodeData n1,nodeData n2)
{
	return sqrt ((n2.x-n1.x)*(n2.x-n1.x)+(n2.y-n1.y)*(n2.y-n1.y)+(n2.z-n1.z)*(n2.z-n1.z));
}
int calculateMatrixMethod(int noOfNodes,nodeData *nodes,int noOfElements,elementData *elements)
{
	int degreesOfFreedom=noOfNodes*6;
	vec3 *nodeCoordinates=(vec3*)malloc(sizeof(vec3)*(1+noOfNodes));
	int *reactionsQ=ivector(1,degreesOfFreedom);	/* allocate memory for reaction data ... */
	int *reactionsR=ivector(1,degreesOfFreedom);	/* allocate memory for reaction data ... */
	float	*rigidRadius=vector(1,noOfNodes);		/* rigid radius around each node */
	for (int i=1;i<=noOfNodes;i++)
	{
		nodeCoordinates[i].x=nodes[i].x;
		nodeCoordinates[i].y=nodes[i].y;
		nodeCoordinates[i].z=nodes[i].z;
		reactionsR[i*6-5]=nodes[i].fixX ;		reactionsQ[i*6-5]=nodes[i].fixX ==0?1:0;
		reactionsR[i*6-4]=nodes[i].fixY ;		reactionsQ[i*6-4]=nodes[i].fixY ==0?1:0;
		reactionsR[i*6-3]=nodes[i].fixZ ;		reactionsQ[i*6-3]=nodes[i].fixZ ==0?1:0;
		reactionsR[i*6-2]=nodes[i].fixXX;		reactionsQ[i*6-2]=nodes[i].fixXX==0?1:0;
		reactionsR[i*6-1]=nodes[i].fixYY;		reactionsQ[i*6-1]=nodes[i].fixYY==0?1:0;
		reactionsR[i*6-0]=nodes[i].fixZZ;		reactionsQ[i*6-0]=nodes[i].fixZZ==0?1:0;
		rigidRadius[i]=(float)nodes[i].rigidRadius;
	}
	int totalReactions=0;
	for (int i=1;i<=degreesOfFreedom;i++)
		totalReactions+=reactionsQ[i];
	for (int i=1;i<=noOfElements;i++)
	{
		elements[i].length=length(nodes[elements[i].node1],nodes[elements[i].node2]);
		elements[i].effectiveLength=elements[i].length-nodes[elements[i].node1].rigidRadius-nodes[elements[i].node2].rigidRadius;
	}
	FILE	*fp;		// input and output file pointer
	float	*EMs=NULL,	// extra inertia
		*NMs=NULL, 	// mass of a node
		*NMx,*NMy,*NMz,	// inertia of a node in global coord	
		pan=1.0,	// >0: pan during animation; 0: don't
		scale=1.0;	// zoom scale for 3D plotting in Gnuplot
	double	**K=NULL,	// equilibrium stiffness matrix
		traceK = 0.0,	// trace of the global stiffness matrix
		**M = NULL,	// global mass matrix
		traceM = 0.0,	// trace of the global mass matrix
		***eqF_mech=NULL,// equivalent end forces from mech loads global
		***eqF_temp=NULL,// equivalent end forces from temp loads global
		**F_mech=NULL,	// mechanical load vectors, all load cases	
		**F_temp=NULL,	// thermal load vectors, all load cases
		*F  = NULL, 	// total load vectors for a load case
		*R  = NULL,	// total reaction force vector
		*dR = NULL,	// incremental reaction force vector 
		*D  = NULL,	// displacement vector
		*dD = NULL,	// incremental displacement vector
		*dF = NULL,	// equilibrium error in nonlinear anlys
		**Q = NULL,	// local member node end-forces
		tol = 1.0e-9,	// tolerance for modal convergence
		shift = 0.0,	// shift-factor for rigid-body-modes
		struct_mass,	// mass of structural system	
		total_mass,	// total structural mass and extra mass 
		*f  = NULL,	// resonant frequencies	
		**V = NULL,	// resonant mode-shapes
		rms_resid=1.0,	// root mean square of residual displ. error
		error = 1.0,	// rms equilibrium error and reactions
		Cfreq = 0.0,	// frequency used for Guyan condensation
		**Kc, **Mc,	// condensed stiffness and mass matrices
		exagg_static=10,// exaggerate static displ. in mesh data
		exagg_modal=10;	// exaggerate modal displ. in mesh data
		// peak internal forces, moments, and displacments
		// in each frame element and each load case 
	double	**pkNx, **pkVy, **pkVz, **pkTx, **pkMy, **pkMz, **pkDx, **pkDy, **pkDz, **pkRx, **pkSy, **pkSz;
int		nI=0,		// number of nodes w/ extra inertia
		nX=0,		// number of elemts w/ extra mass
		nC=0,		// number of condensed nodes
		anlyz=1,	// 1: stiffness analysis, 0: data check	
		nM=0,		// number of desired modes
		Mmethod,	// 1: Subspace Jacobi, 2: Stodola
		nM_calc,	// number of modes to calculate
		lump=1,		// 1: lumped, 0: consistent mass matrix
		iter=0,		// number of iterations	
		ok=1,		// number of (-ve) diag. terms of L D L'
		anim[128],	// the modes to be animated
		Cdof=0,		// number of condensed degrees o freedom
		Cmethod=0,	// matrix condensation method
		*c=NULL,	// vector of DoF's to condense
		*m=NULL,	// vector of modes to condense
		debug=0,	// 1: debugging screen output, 0: none
		verbose=1,	// 1: copious screen output, 0: none
		axial_strain_warning = 0, // 0: "ok", 1: strain > 0.001
		ExitCode = 0;	// error code returned by Frame3DD

	int	shear_flag= -1,	//   over-ride input file value	
		geom_flag = -1,	//   over-ride input file value
		anlyz_flag= -1,	//   over-ride input file value
		D3_flag = -1,	//   over-ride 3D plotting check
		lump_flag = -1,	//   over-ride input file value
		modal_flag= -1,	//   over-ride input file value	
		write_matrix=-1,//   write stiffness and mass matrix
		axial_sign=-1,  //   suppress 't' or 'c' in output data
		condense_flag=-1; // over-ride input file value	

	int	sfrv=0;		// *scanf return value for err checking

	double	exagg_flag=-1.0, // over-ride input file value
		tol_flag  =-1.0, // over-ride input file value
		shift_flag=-1.0; // over-ride input file value

	float	pan_flag = -1.0; // over-ride input file value

	char	extn[16];	// Input Data file name extension
	double *elementLengths=dvector(1,noOfElements);	/* length of each element		*/
	double *effectiveElementLengths=dvector(1,noOfElements);	/* length of each element		*/
	int *elementsNode1=ivector(1,noOfElements);	/* node #2 of each element		*/
	int *elementsNode2=ivector(1,noOfElements);	/* node #2 of each element		*/
	float *elementsAx  =  vector(1,noOfElements);	/* cross section area of each element	*/
	float *elementsAsy =  vector(1,noOfElements);	/* shear area in local y direction 	*/
	float *elementsAsz =  vector(1,noOfElements);	/* shear area in local z direction	*/
	float *elementsJx  =  vector(1,noOfElements);	/* torsional moment of inertia 		*/
	float *elementsIy  =  vector(1,noOfElements);	/* bending moment of inertia about y-axis */
	float *elementsIz  =  vector(1,noOfElements);	/* bending moment of inertia about z-axis */
	float *elementsE   =  vector(1,noOfElements);	/* frame element Young's modulus	*/
	float *elementsG   =  vector(1,noOfElements);	/* frame element shear modulus		*/
	float *elementsp   =  vector(1,noOfElements);	/* element rotation angle about local x axis */
	float *elementsd   =  vector(1,noOfElements);	/* element mass density			*/
	for (int i=1;i<=noOfElements;i++)
	{
		elementsNode1[i]=elements[i].node1;
		elementsNode2[i]=elements[i].node2;
		elementLengths[i]=elements[i].length;
		effectiveElementLengths[i]=elements[i].effectiveLength;
		elementsAx  [i]=(float)elements[i].Ax;
		elementsAsy [i]=(float)elements[i].Asy;
		elementsAsz [i]=(float)elements[i].Asz;
		elementsJx  [i]=(float)elements[i].Jx;
		elementsIy  [i]=(float)elements[i].Iy;
		elementsIz  [i]=(float)elements[i].Iz;
		elementsE   [i]=(float)elements[i].E; 
		elementsG   [i]=(float)elements[i].G; 
		elementsp   [i]=(float)elements[i].rotationAboutX; 
		elementsd   [i]=(float)elements[i].density; 
	
	}
	int shearDeformation=0;
	int geometricNonlinearity=1;
	float xIncrementForInternalForceData=0.;//x-increment for internal force data, if is -1 then internal force calculations are skipped.
	int numberOfLoadCases=1;
	F_mech  = dmatrix(1,numberOfLoadCases,1,degreesOfFreedom);	/* mechanical load vector	*/
	F_temp  = dmatrix(1,numberOfLoadCases,1,degreesOfFreedom);	/* temperature load vector	*/
	F       = dvector(1,degreesOfFreedom);	/* external load vector	*/
	dF	= dvector(1,degreesOfFreedom);	/* equilibrium error {F} - [K]{D} */

	eqF_mech =  D3dmatrix(1,numberOfLoadCases,1,noOfElements,1,12); /* eqF due to mech loads */
	eqF_temp =  D3dmatrix(1,numberOfLoadCases,1,noOfElements,1,12); /* eqF due to temp loads */

	K   = dmatrix(1,degreesOfFreedom,1,degreesOfFreedom);	/* global stiffness matrix	*/
	Q   = dmatrix(1,noOfElements,1,12);	/* end forces for each member	*/

	D   = dvector(1,degreesOfFreedom);	/* displacments of each node		*/
	dD  = dvector(1,degreesOfFreedom);	/* incremental displ. of each node	*/
	R   = dvector(1,degreesOfFreedom);	/* reaction forces			*/
	dR  = dvector(1,degreesOfFreedom);	/* incremental reaction forces		*/

	EMs =  vector(1,noOfElements);	/* lumped mass for each frame element	*/
	NMs =  vector(1,noOfNodes);	/* node mass for each node		*/
	NMx =  vector(1,noOfNodes);	/* node inertia about global X axis	*/
	NMy =  vector(1,noOfNodes);	/* node inertia about global Y axis	*/
	NMz =  vector(1,noOfNodes);	/* node inertia about global Z axis	*/

	c = ivector(1,degreesOfFreedom); 	/* vector of condensed degrees of freedom */
	m = ivector(1,degreesOfFreedom); 	/* vector of condensed mode numbers	*/

	// peak axial forces, shears, torques, and moments along each element
	pkNx = dmatrix(1,numberOfLoadCases,1,noOfElements);	
	pkVy = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkVz = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkTx = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkMy = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkMz = dmatrix(1,numberOfLoadCases,1,noOfElements); 

	// peak displacements and slopes along each element
	pkDx = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkDy = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkDz = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkRx = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkSy = dmatrix(1,numberOfLoadCases,1,noOfElements);
	pkSz = dmatrix(1,numberOfLoadCases,1,noOfElements); 

	float gravitationalAccelerationX=0.;	// gravitational acceleration in global X 
	float gravitationalAccelerationY=0.;	// gravitational acceleration in global Y
	float gravitationalAccelerationZ=0.;	// gravitational acceleration in global Z

int	numberOfNodesWithPrescribedNodalDisplacements[_NL_],	// number of prescribed nodal displ'nts
		nF[_NL_],	// number of loaded nodes
		nU[_NL_],	// number of members w/ unifm dist loads
		nW[_NL_],	// number of members w/ trapz dist loads
		nP[_NL_],	// number of members w/ conc point loads
		nT[_NL_];	// number of members w/ temp. changes

float	***U=NULL,	// uniform distributed member loads
			***W=NULL,	// trapizoidal distributed member loads
			***P=NULL,	// member concentrated loads	
			***T=NULL,	// member temperature  loads
			**prescribedNodalDisplacements=NULL;	// prescribed node displacements
	U   =  D3matrix(1,numberOfLoadCases,1,noOfElements,1,4);    /* uniform load on each member */
	W   =  D3matrix(1,numberOfLoadCases,1,10*noOfElements,1,13);/* trapezoidal load on each member */
	P   =  D3matrix(1,numberOfLoadCases,1,10*noOfElements,1,5); /* internal point load each member */
	T   =  D3matrix(1,numberOfLoadCases,1,noOfElements,1,8);    /* internal temp change each member*/
	prescribedNodalDisplacements  =  matrix(1,numberOfLoadCases,1,degreesOfFreedom); /* prescribed displacement of each node */

	read_and_assemble_loads(/* fp,*/ noOfNodes, noOfElements, numberOfLoadCases, degreesOfFreedom, nodeCoordinates, elementLengths, effectiveElementLengths, elementsNode1, elementsNode2,
			elementsAx,elementsAsy,elementsAsz, elementsIy,elementsIz, elementsE, elementsG, elementsp,
			elementsd, gravitationalAccelerationX, gravitationalAccelerationY, gravitationalAccelerationZ,reactionsR,shearDeformation,
			nF, nU, nW, nP, nT, numberOfNodesWithPrescribedNodalDisplacements,
			Q, F_temp, F_mech, F, U, W, P, T,
			prescribedNodalDisplacements, eqF_mech, eqF_temp, verbose ,nodes);
	//read_mass_data( fp, inFile, noOfNodes, noOfElements, &nI, &nX,
	//		elementsd, EMs, NMs, NMx, NMy, NMz,
	//		elementLengths, elementsAx, &total_mass, &struct_mass, &nM,
	//		&Mmethod, modal_flag, 
	//		&lump, lump_flag, &tol, tol_flag, &shift, shift_flag,
	//		&exagg_modal, modepath, anim, &pan, pan_flag, 
	//		verbose, debug );
	//read_condensation_data( fp, noOfNodes,nM, &nC, &Cdof, &Cmethod, condense_flag, c,m, verbose );
	//char outFile[] = "F:\\development\\#workingDir\\test\\frame3dd\\test.out";
	//fp = fopen(outFile, "a"); /* open the output data file for appending */
	if ( anlyz ) 			/* solve the problem	*/
	{
		srand((int)time(NULL));
		for (int loadCase=1;loadCase<=numberOfLoadCases; loadCase++) 	/* begin load case analysis loop */
		{
			/*  initialize displacements and displ. increment to {0}  */
			/*  initialize reactions     and react. increment to {0}  */
			for (int i=1; i<=degreesOfFreedom; i++)	
				D[i] = dD[i] = R[i] = dR[i] = 0.0;	
			/*  initialize internal element end forces Q = {0}	*/
			for (int i=1; i<=noOfElements; i++)	
			{
				for (int j=1;j<=12;j++)	
					Q[i][j] = 0.0;
			}
			/*  elastic stiffness matrix  [K({D}^(i))], {D}^(0)={0} (i=0) */
			assemble_K ( K, degreesOfFreedom, noOfElements, nodeCoordinates, rigidRadius,elementLengths, effectiveElementLengths, elementsNode1, elementsNode2,elementsAx, elementsAsy, elementsAsz, elementsJx,elementsIy,elementsIz, elementsE, elementsG, elementsp,shearDeformation, geometricNonlinearity, Q, debug );
			/* first apply temperature loads only, if there are any ... */
			if (nT[loadCase] > 0) 
			{
				/*  solve {F_t} = [K({D=0})] * {D_t} */
				solve_system(K,dD,F_temp[loadCase],dR,degreesOfFreedom,reactionsQ,reactionsR,&ok,verbose,&rms_resid);

				/* increment {D_t} = {0} + {D_t} temp.-induced displ */
				for (int i=1; i<=degreesOfFreedom; i++)
				{
					if (reactionsQ[i]) 
						D[i] += dD[i];
				}
				/* increment {R_t} = {0} + {R_t} temp.-induced react */
				for (int i=1; i<=degreesOfFreedom; i++)
				{
					if (reactionsR[i]) 
						R[i] += dR[i];
				}
				if (geometricNonlinearity) 	/* assemble K = Ke + Kg */
				{
					 /* compute   {Q}={Q_t} ... temp.-induced forces     */
					 element_end_forces ( Q, noOfElements, nodeCoordinates, elementLengths, effectiveElementLengths, elementsNode1,elementsNode2,elementsAx, elementsAsy,elementsAsz, elementsJx,elementsIy,elementsIz, elementsE,elementsG, elementsp,eqF_temp[loadCase], eqF_mech[loadCase], D, shearDeformation, geometricNonlinearity,&axial_strain_warning );
					 /* assemble temp.-stressed stiffness [K({D_t})]     */
					 assemble_K ( K, degreesOfFreedom, noOfElements, nodeCoordinates, rigidRadius, elementLengths, effectiveElementLengths, elementsNode1, elementsNode2,elementsAx,elementsAsy,elementsAsz, elementsJx,elementsIy,elementsIz, elementsE, elementsG, elementsp,shearDeformation,geometricNonlinearity, Q, debug );
				}
			}
			/* ... then apply mechanical loads only, if there are any ... */
			if ( nF[loadCase]>0 || nU[loadCase]>0 || nW[loadCase]>0 || nP[loadCase]>0 || numberOfNodesWithPrescribedNodalDisplacements[loadCase]>0 || gravitationalAccelerationX != 0 || gravitationalAccelerationY != 0 || gravitationalAccelerationZ != 0 ) 
			{
				/* incremental displ at react'ns = prescribed displ */
				for (int i=1; i<=degreesOfFreedom; i++)
				{
					if (reactionsR[i]) 
						dD[i] = prescribedNodalDisplacements[loadCase][i];
				}
				/*  solve {F_m} = [K({D_t})] * {D_m}	*/
				solve_system(K,dD,F_mech[loadCase],dR,degreesOfFreedom,reactionsQ,reactionsR,&ok,verbose,&rms_resid);
				/* combine {D} = {D_t} + {D_m}	*/
				for (int i=1; i<=degreesOfFreedom; i++) 
				{
					if (reactionsQ[i])	
						D[i] += dD[i];
					else 
					{		
						D[i]  = prescribedNodalDisplacements[loadCase][i];
						dD[i] = 0.0; 
					}
				}
				/* combine {R} = {R_t} + {R_m} --- for linear systems */
				for (int i=1; i<=degreesOfFreedom; i++)	
				{
					if (reactionsR[i])
						R[i] += dR[i];
				}
			}
			/*  combine {F} = {F_t} + {F_m} */
			for (int i=1; i<=degreesOfFreedom; i++)	
				F[i] = F_temp[loadCase][i] + F_mech[loadCase][i]; 
			/*  element forces {Q} for displacements {D}	*/ 
			element_end_forces ( Q, noOfElements, nodeCoordinates, elementLengths, effectiveElementLengths, elementsNode1,elementsNode2,elementsAx, elementsAsy,elementsAsz, elementsJx,elementsIy,elementsIz, elementsE,elementsG, elementsp, eqF_temp[loadCase], eqF_mech[loadCase], D, shearDeformation, geometricNonlinearity,&axial_strain_warning );
		/*  check the equilibrium error	*/
			error = equilibrium_error ( dF, F, K, D, degreesOfFreedom,reactionsQ,reactionsR );
			/* quasi Newton-Raphson iteration for geometric nonlinearity  */
			if (geometricNonlinearity) 
			{ 
				error = 1.0; 
				ok = 0; 
				iter = 0; 
			} /* re-initialize */
			while ( geometricNonlinearity && error > tol && iter < 500 && ok >= 0) 
			{
				++iter;
				/*  assemble stiffness matrix [K({D}^(i))]	      */
				assemble_K ( K, degreesOfFreedom, noOfElements, nodeCoordinates, rigidRadius, elementLengths, effectiveElementLengths, elementsNode1, elementsNode2,elementsAx,elementsAsy,elementsAsz, elementsJx,elementsIy,elementsIz, elementsE, elementsG, elementsp,shearDeformation,geometricNonlinearity, Q, debug );
				/*  compute equilibrium error, {dF}, at iteration i   */
				/*  {dF}^(i) = {F} - [K({D}^(i))]*{D}^(i)	      */
				/*  convergence criteria = || {dF}^(i) ||  /  || F || */
				error = equilibrium_error ( dF, F, K, D, degreesOfFreedom,reactionsQ,reactionsR);
				/*  Powell-Symmetric-Broyden secant stiffness update  */
				// PSB_update ( Ks, dF, dD, degreesOfFreedom );  /* not helpful?   */
				/*  solve {dF}^(i) = [K({D}^(i))] * {dD}^(i)	      */
				solve_system(K,dD,dF,dR,degreesOfFreedom,reactionsQ,reactionsR,&ok,verbose,&rms_resid);
				if ( ok < 0 ) {	/*  K is not positive definite	      */
					fprintf(stderr,"   The stiffness matrix is not pos-def. \n");
					fprintf(stderr,"   Reduce loads and re-run the analysis.\n");
					ExitCode = 181;
					break;
				}
				/*  increment {D}^(i+1) = {D}^(i) + {dD}^(i)	      */
				for (int i=1; i<=degreesOfFreedom; i++)
				{
					if (reactionsQ[i])
						D[i] += dD[i];
				}
				/*  element forces {Q} for displacements {D}^(i)      */ 
				element_end_forces ( Q, noOfElements, nodeCoordinates, elementLengths, effectiveElementLengths, elementsNode1,elementsNode2,elementsAx, elementsAsy,elementsAsz, elementsJx,elementsIy,elementsIz, elementsE,elementsG, elementsp, eqF_temp[loadCase], eqF_mech[loadCase], D, shearDeformation, geometricNonlinearity,&axial_strain_warning );
			}			
			/* end quasi Newton-Raphson iteration */
			/*   strain limit failure ... */
			if (axial_strain_warning > 0 && ExitCode == 0)   
				ExitCode = 182;
			/*   strain limit _and_ buckling failure ... */
			if (axial_strain_warning > 0 && ExitCode == 181)
				ExitCode = 183;

			if ( geometricNonlinearity )
				compute_reaction_forces( R,F,K, D, degreesOfFreedom,reactionsR);
			/*  display RMS equilibrium error */
			//if ( verbose && ok >= 0 ) 
			//	evaluate ( error, rms_resid, tol, geometricNonlinearity );
			for (int i=1;i<=noOfNodes;i++)
			{
				nodes[i].displacementX=D[6*i-5];
				nodes[i].displacementY=D[6*i-4];
				nodes[i].displacementZ=D[6*i-3];
				nodes[i].rotationX=D[6*i-2];
				nodes[i].rotationY=D[6*i-1];
				nodes[i].rotationZ=D[6*i-0];
			}

			//write_static_results ( fp, noOfNodes,noOfElements,numberOfLoadCases,loadCase, degreesOfFreedom, elementsNode1,elementsNode2,F,D,R,reactionsR,Q, rms_resid, ok, axial_sign );

		//write_internal_forces ( outFile, fp, infcpath,loadCase, numberOfLoadCases, title, xIncrementForInternalForceData, nodeCoordinates,
		//			Q, noOfNodes, noOfElements, elementLengths, elementsNode1, elementsNode2, 
		//			elementsAx, elementsAsy, elementsAsz, elementsJx, elementsIy, elementsIz, elementsE, elementsG, elementsp,
		//			elementsd, gravitationalAccelerationX, gravitationalAccelerationY, gravitationalAccelerationZ,
		//			nU[loadCase],U[loadCase],nW[loadCase],W[loadCase],nP[loadCase],P[loadCase],
		//			D, shearDeformation, error );

		//static_mesh ( inFile, infcpath, meshpath, plotpath, title,
		//			noOfNodes, noOfElements, numberOfLoadCases, loadCase, degreesOfFreedom,
		//			nodeCoordinates, elementLengths, elementsNode1,elementsNode2, elementsp, D,
		//			exagg_static, D3_flag, anlyz,
		//			xIncrementForInternalForceData, scale );

	 } /* end load case loop */
	} 
	else 
	{		/*  data check only  */
		 //static_mesh ( inFile, infcpath, meshpath, plotpath, title,noOfNodes, noOfElements, numberOfLoadCases, 0, degreesOfFreedom,nodeCoordinates, elementLengths, elementsNode1,elementsNode2, elementsp, D,exagg_static, D3_flag, anlyz, xIncrementForInternalForceData, scale );
	}


//	if ( nM > 0 )  /* carry out modal analysis */
//	{
//		nM_calc = (nM+8)<(2*nM) ? nM+8 : 2*nM;		/* Bathe */
//		M   = dmatrix(1,degreesOfFreedom,1,degreesOfFreedom);
//		f   = dvector(1,nM_calc);
//		V   = dmatrix(1,degreesOfFreedom,1,nM_calc);
//		assemble_M ( M, degreesOfFreedom, noOfNodes, noOfElements, nodeCoordinates, rigidRadius, elementLengths, elementsNode1,elementsNode2,elementsAx, elementsJx,elementsIy,elementsIz, elementsp, elementsd, EMs, NMs, NMx, NMy, NMz,lump, debug );
//#ifdef MATRIX_DEBUG
//		save_dmatrix ( "Mf", M, 1,degreesOfFreedom, 1,degreesOfFreedom, 0, "w" ); /* free mass matrix */
//#endif
//		for (int j=1; j<=degreesOfFreedom; j++)  /*  compute traceK and traceM */
//		{
//			if ( !reactionsR[j] ) {
//				traceK += K[j][j];
//				traceM += M[j][j];
//			}
//		}
//		for (int i=1; i<=degreesOfFreedom; i++)  /*  modify K and M for reactions    */
//		{
//			if (reactionsR[i] ) {	/* apply reactions to upper triangle */
//				K[i][i] = traceK * 1e4;
//				M[i][i] = traceM;
//				for (int j=i+1; j<=degreesOfFreedom; j++)
//					K[j][i]=K[i][j]=M[j][i]=M[i][j] = 0.0;
//				}
//		}
//		if ( write_matrix ) {	/* write Kd and Md matrices */
//			save_ut_dmatrix ( "Kd", K, degreesOfFreedom, "w" );/* dynamic stff matx */
//			save_ut_dmatrix ( "Md", M, degreesOfFreedom, "w" );/* dynamic mass matx */
//		}
//		if ( anlyz ) 	/* subspace or stodola methods */
//		{
//			if( Mmethod == 1 )
//				subspace( K, M, degreesOfFreedom, nM_calc, f, V, tol,shift,&iter,&ok, verbose );
//			if( Mmethod == 2 )
//				stodola ( K, M, degreesOfFreedom, nM_calc, f, V, tol,shift,&iter,&ok, verbose );
//			for (int j=1; j<=nM_calc; j++) 
//				f[j] = sqrt(f[j])/(2.0*PI);
//			write_modal_results ( fp, noOfNodes,noOfElements,nI, degreesOfFreedom, M,f,V,total_mass, struct_mass,iter, sumR, nM, shift, lump, tol, ok );
//		}
//	}

	//fprintf(fp,"\n");
	//fclose (fp);

	//if ( nM > 0 && anlyz ) 	/* write modal analysis results */
	//{
	//	modal_mesh ( inFile, meshpath, modepath, plotpath, title,noOfNodes,noOfElements, degreesOfFreedom, nM, nodeCoordinates, elementLengths, elementsNode1,elementsNode2, elementsp,M, f, V, exagg_modal, D3_flag, anlyz );
	//	animate ( inFile, meshpath, modepath, plotpath, title,anim,noOfNodes,noOfElements, degreesOfFreedom, nM, nodeCoordinates, elementLengths, elementsp, elementsNode1,elementsNode2, f,V, exagg_modal, D3_flag, pan, scale );
	//}

	//if ( nC > 0 ) 		/* matrix condensation of stiffness and mass */
	//{
	//	if ( verbose ) fprintf(stdout,"\n Matrix Condensation ...\n");

	//	if(Cdof > nM && Cmethod == 3){
	//		fprintf(stderr,"  Cdof > nM ... Cdof = %d  nM = %d \n",
	//			 Cdof, nM );
	//		fprintf(stderr,"  The number of condensed degrees of freedom");
	//		fprintf(stderr," may not exceed the number of computed modes");
	//		fprintf(stderr," when using dynamic condensation.\n");
	//		exit(94);
	//	}

	//	Kc = dmatrix(1,Cdof,1,Cdof);
	//	Mc = dmatrix(1,Cdof,1,Cdof);

	//	if ( m[1] > 0 && nM > 0 )	Cfreq = f[m[1]];

	//	if ( Cmethod == 1 && anlyz) {	/* static condensation only */
	//		static_condensation(K, degreesOfFreedom, c, Cdof, Kc, 0 );
	//		if ( verbose )
	//			fprintf(stdout,"   static condensation of K complete\n");
	//	}
	//	if ( Cmethod == 2 && anlyz ) {  /*  dynamic condensation  */
	//		paz_condensation(M, K, degreesOfFreedom, c, Cdof, Mc,Kc, Cfreq, 0 ); 
	//		if ( verbose ) {
	//			fprintf(stdout,"   Paz condensation of K and M complete");
	//			fprintf(stdout," ... dynamics matched at %f Hz.\n", Cfreq );
	//		}
	//	}
	//	if ( Cmethod == 3 && nM > 0 && anlyz ) 
	//	{
	//		modal_condensation(M,K, degreesOfFreedom,reactionsR, c, Cdof, Mc,Kc, V,f, m, 0 );
	//	}
	//	save_dmatrix("Kc", Kc, 1,Cdof, 1,Cdof, 0, "w" );
	//	save_dmatrix("Mc", Mc, 1,Cdof, 1,Cdof, 0, "w" );

	//	free_dmatrix(Kc, 1,Cdof,1,Cdof );
	//	free_dmatrix(Mc, 1,Cdof,1,Cdof );
	//}


	/* deallocate memory used for each frame analysis variable */
	deallocate ( noOfNodes, noOfElements, numberOfLoadCases, nF, nU, nW, nP, nT, degreesOfFreedom, nM,
			nodeCoordinates, rigidRadius, elementLengths, effectiveElementLengths, elementsNode1, elementsNode2,reactionsQ,reactionsR,
			elementsAx, elementsAsy, elementsAsz, elementsJx, elementsIy, elementsIz, elementsE, elementsG, elementsp,
			U,W,P,T, prescribedNodalDisplacements, F_mech, F_temp,
			eqF_mech, eqF_temp, F, dF, 
			K, Q, D, dD, R, dR, 
			elementsd,EMs,NMs,NMx,NMy,NMz, M,f,V, c, m, 
			pkNx, pkVy, pkVz, pkTx, pkMy, pkMz,
			pkDx, pkDy, pkDz, pkRx, pkSy, pkSz
	);

	//if ( verbose ) fprintf(stdout,"\n");

	//if ( argc == 1 ) 
	//{ /* wait for keyboard entry to close the terminal */
	//	 fprintf(stderr," The Output Data was appended to %s \n", outFile );
	//	 fprintf(stderr," A Gnuplot script was written to %s \n", plotpath );
	//	 fprintf(stderr," Press the 'Enter' key to close.\n");
	//	 (void) getchar();	// clear the buffer ?? 
	//	 while( !getchar() ) ;	// wait for the Enter key to be hit 
	//}
	//color(0);

	return( ExitCode );
}