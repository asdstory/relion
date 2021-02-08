/***************************************************************************
 *
 * Author: "Sjors H.W. Scheres"
 * MRC Laboratory of Molecular Biology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * This complete copyright notice must be included in any revised version of the
 * source code. Additional authorship citations may be added, but existing
 * author citations must be preserved.
 ***************************************************************************/


#ifndef SRC_PIPELINE_JOBS_H_
#define SRC_PIPELINE_JOBS_H_

#define JOBOPTION_UNDEFINED 0
#define JOBOPTION_ANY 1
#define JOBOPTION_FILENAME 2
#define JOBOPTION_INPUTNODE 3
#define JOBOPTION_RADIO 4
#define JOBOPTION_BOOLEAN 5
#define JOBOPTION_SLIDER 6
#define JOBOPTION_ONLYTEXT 7
#include "src/macros.h"
#include "src/metadata_table.h"
#include "src/filename.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#define YSTEP 20

#define TOGGLE_DEACTIVATE 0
#define TOGGLE_REACTIVATE 1
#define TOGGLE_ALWAYS_DEACTIVATE 2
#define TOGGLE_LEAVE_ACTIVE 3

#define RADIO_SAMPLING 0
#define RADIO_NODETYPE 1
#define RADIO_GAIN_ROTATION 2
#define RADIO_GAIN_FLIP 3

// Our own defaults at LMB are the hard-coded ones
#define DEFAULTQSUBLOCATION "/public/EM/RELION/relion/bin/relion_qsub.csh"
#define DEFAULTCTFFINDLOCATION "/public/EM/ctffind/ctffind.exe"
#define DEFAULTMOTIONCOR2LOCATION "/public/EM/MOTIONCOR2/MotionCor2"
#define DEFAULTGCTFLOCATION "/public/EM/Gctf/bin/Gctf"
#define DEFAULTTOPAZLOCATION "/public/EM/RELION/topaz"
#define DEFAULTRESMAPLOCATION "/public/EM/ResMap/ResMap-1.1.4-linux64"
#define DEFAULTQSUBCOMMAND "qsub"
#define DEFAULTQUEUENAME "openmpi"
#define DEFAULTMININIMUMDEDICATED 1
#define DEFAULTWARNINGLOCALMPI 32
#define DEFAULTALLOWCHANGEMINDEDICATED true
#define DEFAULTQUEUEUSE false
#define DEFAULTNRMPI 1
#define DEFAULTMPIMAX 64
#define DEFAULTNRTHREADS 1
#define DEFAULTTHREADMAX 16
#define DEFAULTMPIRUN "mpirun"
#define DEFAULTSCRATCHDIR ""

static const std::vector<std::string> job_undefined_options{
	"undefined"
};

static const std::vector<std::string> job_boolean_options{
	"Yes",
	"No"
};

static const std::vector<std::string> job_sampling_options{
	"30 degrees",
	"15 degrees",
	"7.5 degrees",
	"3.7 degrees",
	"1.8 degrees",
	"0.9 degrees",
	"0.5 degrees",
	"0.2 degrees",
	"0.1 degrees"
};
// Modify the loop in JobOption::getHealPixOrder when you add more choices!

static const std::vector<std::string> job_nodetype_options{
	"Particle coordinates (*.box, *_pick.star)",
	"Particles STAR file (.star)",
	"Multiple (2D or 3D) references (.star or .mrcs)",
	"Micrographs STAR file (.star)",
	"3D reference (.mrc)",
	"3D mask (.mrc)",
	"Unfiltered half-map (unfil.mrc)"
};

static const std::vector<std::string> job_nodetype_options_tomo{
	"Set of tomograms STAR file (.star)",
	"Particles STAR file (.star)",
	"Multiple (2D or 3D) references (.star or .mrcs)",
	"3D reference (.mrc)",
	"3D mask (.mrc)",
	"Unfiltered half-map (unfil.mrc)"
};

static const std::vector<std::string> job_gain_rotation_options{
	"No rotation (0)",
	"90 degrees (1)",
	"180 degrees (2)",
	"270 degrees (3)"
};

static const std::vector<std::string> job_gain_flip_options{
	"No flipping (0)",
	"Flip upside down (1)",
	"Flip left to right (2)"
};

static const std::vector<std::string> job_ctffit_options{
	"No",
	"Per-micrograph",
	"Per-particle"
};

// To have a line on the GUI to change the minimum number of dedicated in a job
static bool do_allow_change_minimum_dedicated;

// Optional output file for any jobtype that explicitly defines the output nodes
#define RELION_OUTPUT_NODES "RELION_OUTPUT_NODES.star"

/*
 * The Node class represents data and metadata that are either input to or output from Processes
 * Nodes are connected to each by Edges:
 * - the fromEdgeList are connections with Nodes earlier (higher up) in the pipeline
 * - the toEdgeList are connections with Nodes later (lower down) in the pipeline
 *
 * Nodes could be of the following types:
 */
#define NODE_MOVIES			0 // 2D micrograph movie(s), e.g. Falcon001_movie.mrcs or micrograph_movies.star
#define NODE_MICS			1 // 2D micrograph(s), possibly with CTF information as well, e.g. Falcon001.mrc or micrographs.star
#define NODE_MIC_COORDS		2 // Suffix for particle coordinates in micrographs (e.g. autopick.star or .box)
#define NODE_PART_DATA		3 // A metadata (STAR) file with particles (e.g. particles.star or run1_data.star)
//#define NODE_MOVIE_DATA		4 // A metadata (STAR) file with particle movie-frames (e.g. particles_movie.star or run1_ct27_data.star)
#define NODE_REFS       	5 // A STAR file with one or multiple references, e.g. autopick_references.star
#define NODE_3DREF       	6 // A single 3D-reference, e.g. map.mrc
#define NODE_MASK			7 // 3D mask, e.g. mask.mrc or masks.star
// SHWS 28nov2019: NODE_MODEL should disappear now, only here for backwards compatibility
#define NODE_MODEL		    8 // A model STAR-file for class selection
#define NODE_OPTIMISER		9 // An optimiser STAR-file for job continuation
#define NODE_HALFMAP		10// Unfiltered half-maps from 3D auto-refine, e.g. run1_half?_class001_unfil.mrc
// SHWS 28nov2019: NODE_FINALMAP should disappear now, only here for backwards compatibility
#define NODE_FINALMAP		11// Sharpened final map from post-processing (cannot be used as input)
#define NODE_RESMAP			12// Resmap with local resolution (cannot be used as input)
#define NODE_PDF_LOGFILE    13// PDF logfile
#define NODE_POST           14// Postprocess STAR file (with FSC curve, unfil half-maps, masks etc in it: used by Jasenko's programs
#define NODE_POLISH_PARAMS  15// Txt file with optimal parameters for Bayesian polishing

#define NODE_TOMO_OPTIMISATION 50 // Jasenko's combined optimisation set for subtomogram averaging
#define NODE_TOMO_TOMOGRAMS    51 // Jasenko's set of tomograms
#define NODE_TOMO_TRAJECTORIES 52 // Jasenko's definition of subtomogram motion trajectories
#define NODE_TOMO_MANIFOLDS    53 // Jasenko's definition of 3D shapes (for picking of subtomograms)


#define NODE_MOVIES_LABEL   	 "relion.MovieStar"
#define NODE_MICS_LABEL			 "relion.MicrographStar"
#define NODE_MIC_COORDS_LABEL	 "relion.CoordinateStar"
#define NODE_PART_DATA_LABEL	 "relion.ParticleStar"
#define NODE_REFS_LABEL          "relion.ReferenceStar"
#define NODE_3DREF_LABEL       	 "relion.DensityMap"
#define NODE_MASK_LABEL			 "relion.Mask"
#define NODE_OPTIMISER_LABEL	 "relion.OptimiserStar"
#define NODE_HALFMAP_LABEL		 "relion.HalfMap"
#define NODE_RESMAP_LABEL		 "relion.LocalResolutionMap"
#define NODE_PDF_LOGFILE_LABEL   "relion.PdfLogfile"
#define NODE_POST_LABEL          "relion.PostprocessStar"
#define NODE_POLISH_PARAMS_LABEL "relion.PolishParams"
#define NODE_TOMO_OPTIMISATION_LABEL "relion.TomoOptimisationSet"
#define NODE_TOMO_TOMOGRAMS_LABEL    "relion.TomoTomogramSet"
#define NODE_TOMO_TRAJECTORIES_LABEL "relion.TomoTrajectorySet"
#define NODE_TOMO_MANIFOLDS_LABEL    "relion.TomoManifoldSet"

static std::map<int, std::string> node_type2label = {{NODE_MOVIES, NODE_MOVIES_LABEL},
		{NODE_MICS, NODE_MICS_LABEL},
		{NODE_MIC_COORDS, NODE_MIC_COORDS_LABEL},
		{NODE_PART_DATA, NODE_PART_DATA_LABEL},
		{NODE_REFS, NODE_REFS_LABEL},
		{NODE_3DREF, NODE_3DREF_LABEL},
		{NODE_MASK, NODE_MASK_LABEL},
		{NODE_MODEL, NODE_OPTIMISER_LABEL}, // SHWS 27nov2019: no longer distinguish between MODEL and OPTIMISER nodes ... keep integer NODE for backwards compatibility
		{NODE_OPTIMISER, NODE_OPTIMISER_LABEL},
		{NODE_HALFMAP, NODE_HALFMAP_LABEL},
		{NODE_FINALMAP, NODE_3DREF_LABEL}, // SHWS 27nov2019: no longer distinguish between 3DREF and FINAL maps ... keep integer NODE for backwards compatibility
		{NODE_RESMAP, NODE_3DREF_LABEL},
		{NODE_PDF_LOGFILE, NODE_PDF_LOGFILE_LABEL},
		{NODE_POST, NODE_POST_LABEL},
		{NODE_POLISH_PARAMS, NODE_POLISH_PARAMS_LABEL},
		{NODE_TOMO_OPTIMISATION, NODE_TOMO_OPTIMISATION_LABEL},
		{NODE_TOMO_TOMOGRAMS, NODE_TOMO_TOMOGRAMS_LABEL},
		{NODE_TOMO_TRAJECTORIES, NODE_TOMO_TRAJECTORIES_LABEL},
		{NODE_TOMO_MANIFOLDS, NODE_TOMO_MANIFOLDS_LABEL} };

static std::map<std::string, int> node_label2type = {{NODE_MOVIES_LABEL, NODE_MOVIES},
		{NODE_MICS_LABEL, NODE_MICS},
		{NODE_MIC_COORDS_LABEL, NODE_MIC_COORDS},
		{NODE_PART_DATA_LABEL, NODE_PART_DATA},
		{NODE_REFS_LABEL, NODE_REFS},
		{NODE_3DREF_LABEL, NODE_3DREF},
		{NODE_MASK_LABEL, NODE_MASK},
//		{NODE_MODEL_LABEL, NODE_MODEL},
		{NODE_OPTIMISER_LABEL, NODE_OPTIMISER},
		{NODE_HALFMAP_LABEL, NODE_HALFMAP},
//		{NODE_FINALMAP_LABEL, NODE_FINALMAP},
		{NODE_RESMAP_LABEL, NODE_RESMAP},
		{NODE_PDF_LOGFILE_LABEL, NODE_PDF_LOGFILE},
		{NODE_POST_LABEL, NODE_POST},
		{NODE_POLISH_PARAMS_LABEL, NODE_POLISH_PARAMS},
		{NODE_TOMO_OPTIMISATION_LABEL, NODE_TOMO_OPTIMISATION},
		{NODE_TOMO_TOMOGRAMS_LABEL,    NODE_TOMO_TOMOGRAMS},
		{NODE_TOMO_TRAJECTORIES_LABEL, NODE_TOMO_TRAJECTORIES},
		{NODE_TOMO_MANIFOLDS_LABEL,    NODE_TOMO_MANIFOLDS} };



// All the directory names of the different types of jobs defined inside the pipeline
#define PROC_IMPORT_LABEL        "Import"       // Import any file as a Node of a given type
#define PROC_MOTIONCORR_LABEL 	 "MotionCorr"   // Import any file as a Node of a given type
#define PROC_CTFFIND_LABEL	     "CtfFind"  	   // Estimate CTF parameters from micrographs for either entire micrographs and/or particles
#define PROC_MANUALPICK_LABEL    "ManualPick"   // Manually pick particle coordinates from micrographs
#define PROC_AUTOPICK_LABEL		 "AutoPick"     // Automatically pick particle coordinates from micrographs, their CTF and 2D references
#define PROC_EXTRACT_LABEL		 "Extract"      // Window particles, normalize, downsize etc from micrographs (also combine CTF into metadata file)
#define PROC_CLASSSELECT_LABEL   "Select" 	   // Read in model.star file, and let user interactively select classes through the display (later: auto-selection as well)
#define PROC_2DCLASS_LABEL 		 "Class2D"      // 2D classification (from input particles)
#define PROC_3DCLASS_LABEL		 "Class3D"      // 3D classification (from input 2D/3D particles, an input 3D-reference, and possibly a 3D mask)
#define PROC_3DAUTO_LABEL        "Refine3D"     // 3D auto-refine (from input particles, an input 3Dreference, and possibly a 3D mask)
//#define PROC_POLISH_LABEL	     "Polish"       // Particle-polishing (from movie-particles)
#define PROC_MASKCREATE_LABEL    "MaskCreate"   // Process to create masks from input maps
#define PROC_JOINSTAR_LABEL      "JoinStar"     // Process to create masks from input maps
#define PROC_SUBTRACT_LABEL      "Subtract"     // Process to subtract projections of parts of the reference from experimental images
#define PROC_POST_LABEL			 "PostProcess"  // Post-processing (from unfiltered half-maps and a possibly a 3D mask)
#define PROC_RESMAP_LABEL  	     "LocalRes"     // Local resolution estimation (from unfiltered half-maps and a 3D mask)
//#define PROC_MOVIEREFINE_LABEL "MovieRefine"  // Movie-particle extraction and refinement combined
#define PROC_INIMODEL_LABEL		 "InitialModel" // De-novo generation of 3D initial model (using SGD)
#define PROC_MULTIBODY_LABEL	 "MultiBody"    // Multi-body refinement
#define PROC_MOTIONREFINE_LABEL  "Polish"       // Jasenko's motion fitting program for Bayesian polishing (to replace MovieRefine?)
#define PROC_CTFREFINE_LABEL     "CtfRefine"    // Jasenko's program for defocus and beamtilt optimisation
#define PROC_TOMO_IMPORT_LABEL      "ImportTomo"              // Import for tomography GUI
#define PROC_TOMO_SUBTOMO_LABEL     "PseudoSubtomo"           // Creation of pseudo-subtomograms from tilt series images
#define PROC_TOMO_CTFREFINE_LABEL   "CtfRefineTomo"           // CTF refinement (defocus & aberrations) for tomography
#define PROC_TOMO_ALIGN_LABEL       "FrameAlignTomo"          // Frame alignment and particle polishing for subtomography
#define PROC_TOMO_RECONSTRUCT_LABEL "ReconstructParticleTomo" // Calculation of particle average from the individual tilt series images
#define PROC_EXTERNAL_LABEL      "External"     // For running non-relion programs


#define PROC_IMPORT         0 // Import any file as a Node of a given type
#define PROC_MOTIONCORR 	1 // Import any file as a Node of a given type
#define PROC_CTFFIND	    2 // Estimate CTF parameters from micrographs for either entire micrographs and/or particles
#define PROC_MANUALPICK 	3 // Manually pick particle coordinates from micrographs
#define PROC_AUTOPICK		4 // Automatically pick particle coordinates from micrographs, their CTF and 2D references
#define PROC_EXTRACT		5 // Window particles, normalize, downsize etc from micrographs (also combine CTF into metadata file)
//#define PROC_SORT         6 // Sort particles based on their Z-scores
#define PROC_CLASSSELECT    7 // Read in model.star file, and let user interactively select classes through the display (later: auto-selection as well)
#define PROC_2DCLASS		8 // 2D classification (from input particles)
#define PROC_3DCLASS		9 // 3D classification (from input 2D/3D particles, an input 3D-reference, and possibly a 3D mask)
#define PROC_3DAUTO         10// 3D auto-refine (from input particles, an input 3Dreference, and possibly a 3D mask)
//#define PROC_POLISH  		11// Particle-polishing (from movie-particles)
#define PROC_MASKCREATE     12// Process to create masks from input maps
#define PROC_JOINSTAR       13// Process to create masks from input maps
#define PROC_SUBTRACT       14// Process to subtract projections of parts of the reference from experimental images
#define PROC_POST			15// Post-processing (from unfiltered half-maps and a possibly a 3D mask)
#define PROC_RESMAP 		16// Local resolution estimation (from unfiltered half-maps and a 3D mask)
//#define PROC_MOVIEREFINE    17// Movie-particle extraction and refinement combined
#define PROC_INIMODEL		18// De-novo generation of 3D initial model (using SGD)
#define PROC_MULTIBODY      19// Multi-body refinement
#define PROC_MOTIONREFINE   20// Jasenko's motion_refine
#define PROC_CTFREFINE      21// Jasenko's ctf_refine
#define PROC_TOMO_IMPORT        50// Import for tomography GUI
#define PROC_TOMO_SUBTOMO   51// Creation of pseudo-subtomograms from tilt series images
#define PROC_TOMO_CTFREFINE     52// CTF refinement (defocus & aberrations for tomography)
#define PROC_TOMO_ALIGN        53// Frame alignment and particle polishing for subtomography
#define PROC_TOMO_RECONSTRUCT       54// Calculation of particle average from the individual tilt series images
#define PROC_EXTERNAL       99// External scripts


static std::map<int, std::string> proc_type2label = {{PROC_IMPORT, PROC_IMPORT_LABEL},
		{PROC_MOTIONCORR, PROC_MOTIONCORR_LABEL},
		{PROC_CTFFIND, PROC_CTFFIND_LABEL},
		{PROC_MANUALPICK, PROC_MANUALPICK_LABEL},
		{PROC_AUTOPICK, PROC_AUTOPICK_LABEL},
		{PROC_EXTRACT, PROC_EXTRACT_LABEL},
		{PROC_CLASSSELECT, PROC_CLASSSELECT_LABEL},
		{PROC_2DCLASS, PROC_2DCLASS_LABEL},
		{PROC_3DCLASS, PROC_3DCLASS_LABEL},
		{PROC_3DAUTO, PROC_3DAUTO_LABEL},
		{PROC_MASKCREATE,        PROC_MASKCREATE_LABEL},
		{PROC_JOINSTAR,        PROC_JOINSTAR_LABEL},
		{PROC_SUBTRACT,        PROC_SUBTRACT_LABEL},
		{PROC_POST,             PROC_POST_LABEL},
		{PROC_RESMAP,           PROC_RESMAP_LABEL},
		{PROC_INIMODEL,         PROC_INIMODEL_LABEL},
		{PROC_MULTIBODY,        PROC_MULTIBODY_LABEL},
		{PROC_MOTIONREFINE,     PROC_MOTIONREFINE_LABEL},
		{PROC_CTFREFINE,        PROC_CTFREFINE_LABEL},
		{PROC_TOMO_IMPORT,      PROC_TOMO_IMPORT_LABEL},
		{PROC_TOMO_SUBTOMO,     PROC_TOMO_SUBTOMO_LABEL},
		{PROC_TOMO_CTFREFINE,   PROC_TOMO_CTFREFINE_LABEL},
		{PROC_TOMO_ALIGN,       PROC_TOMO_ALIGN_LABEL},
		{PROC_TOMO_RECONSTRUCT, PROC_TOMO_RECONSTRUCT_LABEL},
		{PROC_EXTERNAL,         PROC_EXTERNAL_LABEL}};

static std::map<std::string, int> proc_label2type = {{PROC_IMPORT_LABEL, PROC_IMPORT},
		{PROC_MOTIONCORR_LABEL, PROC_MOTIONCORR},
		{PROC_CTFFIND_LABEL, PROC_CTFFIND},
		{PROC_MANUALPICK_LABEL, PROC_MANUALPICK},
		{PROC_AUTOPICK_LABEL, PROC_AUTOPICK},
		{PROC_EXTRACT_LABEL, PROC_EXTRACT},
		{PROC_CLASSSELECT_LABEL, PROC_CLASSSELECT},
		{PROC_2DCLASS_LABEL, PROC_2DCLASS},
		{PROC_3DCLASS_LABEL, PROC_3DCLASS},
		{PROC_3DAUTO_LABEL,           PROC_3DAUTO},
		{PROC_MASKCREATE_LABEL,       PROC_MASKCREATE},
		{PROC_JOINSTAR_LABEL,         PROC_JOINSTAR},
		{PROC_SUBTRACT_LABEL,         PROC_SUBTRACT},
		{PROC_POST_LABEL,             PROC_POST},
		{PROC_RESMAP_LABEL,           PROC_RESMAP},
		{PROC_INIMODEL_LABEL,         PROC_INIMODEL},
		{PROC_MULTIBODY_LABEL,        PROC_MULTIBODY},
		{PROC_MOTIONREFINE_LABEL,     PROC_MOTIONREFINE},
		{PROC_CTFREFINE_LABEL,        PROC_CTFREFINE},
		{PROC_TOMO_IMPORT_LABEL,      PROC_TOMO_IMPORT},
		{PROC_TOMO_SUBTOMO_LABEL,     PROC_TOMO_SUBTOMO},
		{PROC_TOMO_CTFREFINE_LABEL,   PROC_TOMO_CTFREFINE},
		{PROC_TOMO_ALIGN_LABEL,       PROC_TOMO_ALIGN},
		{PROC_TOMO_RECONSTRUCT_LABEL, PROC_TOMO_RECONSTRUCT},
		{PROC_EXTERNAL_LABEL,         PROC_EXTERNAL}};

// Status a Process may have
#define PROC_RUNNING          0 // (hopefully) running
#define PROC_SCHEDULED        1 // scheduled for future execution
#define PROC_FINISHED_SUCCESS 2 // successfully finished
#define PROC_FINISHED_FAILURE 3 // reported an error
#define PROC_FINISHED_ABORTED 4 // aborted by the user

static std::map<std::string, int> procstatus_label2type = {
		{"Running", PROC_RUNNING},
		{"Scheduled", PROC_SCHEDULED},
		{"Succeeded", PROC_FINISHED_SUCCESS},
		{"Failed", PROC_FINISHED_FAILURE},
		{"Aborted", PROC_FINISHED_ABORTED}};

static std::map<int, std::string> procstatus_type2label = {
		{PROC_RUNNING, "Running", },
		{PROC_SCHEDULED, "Scheduled", },
		{PROC_FINISHED_SUCCESS, "Succeeded"},
		{PROC_FINISHED_FAILURE, "Failed"},
		{PROC_FINISHED_ABORTED, "Aborted"}};


#define HAS_NOT 0
#define HAS_OPTIONAL 1
#define HAS_COMPULSORY 2

struct gui_layout
{
    /// Name for the tab
    std::string tabname;
    /// y-position
    int ypos;
    ///
    RFLOAT w;
};

class Node
{
	public:
	std::string name; // what's my name?
	int type; // which type of node am I?
	std::vector<long int> inputForProcessList; 	  //list of processes that use this Node as input
	long int outputFromProcess;   //Which process made this Node

	// Constructor
	Node(std::string _name, int _type)
	{
		name = _name;
		type = _type;
		outputFromProcess = -1;
	}

	// Destructor
	// Do not delete the adjacent nodes here... They will be deleted by graph destructor
	~Node()
	{
		inputForProcessList.clear();
	}

};

// Helper function to get the outputnames of refine jobs
std::vector<Node> getOutputNodesRefine(std::string outputname, int iter, int K, int dim, int nr_bodies=1);

// One class to store any type of Option for a GUI entry
class JobOption
{
public:
	// Get HealPix order from string. Returns -1 when failed.
	static int getHealPixOrder(std::string s);

	// Get a f/p/m character for CTF fitting. Returns "" when failed.
	static std::string getCtfFitString(std::string option);

public:

	std::string label;
	std::string label_gui;
	int joboption_type;
	std::string variable;
	std::string value;
	std::string default_value;
	std::string helptext;
	float min_value;
	float max_value;
	float step_value;
	int node_type;
	std::string pattern;
	std::string directory;
	std::vector<std::string> radio_options;

public:

	// Any constructor
	JobOption(std::string _label, std::string _default_value, std::string _helptext);

	// FileName constructor
	JobOption(std::string _label, std::string  _default_value, std::string _pattern, std::string _directory, std::string _helptext);

	// InputNode constructor
	JobOption(std::string _label, int _nodetype, std::string _default_value, std::string _pattern, std::string _helptext);

	// Radio constructor
	JobOption(std::string _label, std::vector<std::string> radio_options, int ioption,  std::string _helptext);

	// Boolean constructor
	JobOption(std::string _label, bool _boolvalue, std::string _helptext);

	// Slider constructor
	JobOption(std::string _label, float _default_value, float _min_value, float _max_value, float _step_value, std::string _helptext);

	// Write to a STAR file
	void writeToMetaDataTable(MetaDataTable& MD) const;

	// Empty constructor
	JobOption() { clear(); }

	// Empty destructor
	~JobOption() { clear(); }

	void clear();

	// Set values of label, value, default_value and helptext (common for all types)
	void initialise(std::string _label, std::string _default_value, std::string _helptext);

	// Contains $$ for SchedulerVariable
	bool isSchedulerVariable();

	// Get a string value
	std::string getString();

	// Set a string value
	void setString(std::string set_to);

	// Get a string value
	Node getNode();

	// Get a numbered value
	float getNumber(std::string &errmsg);

	// Get a boolean value
	bool getBoolean();

	// Read value from an ifstream. Return false if cannot find it
	bool readValue(std::ifstream& in);

	// Write value to an ostream
	void writeValue(std::ostream& out);
};

class RelionJob
{

public:

	// The name of this job
	std::string outputName;

	// The alias to this job
	std::string alias;

	// Name of the hidden file
	std::string hidden_name;

	// Which job type is this?
	int type;

	// Is this a continuation job?
	bool is_continue;

	// Activate custom stuff for tomo
	bool do_tomo;

	// List of Nodes of input to this process
	std::vector<Node> inputNodes;

	// List of Nodes of output from this process
	std::vector<Node> outputNodes;

	// All the options to this job
	std::map<std::string, JobOption > joboptions;

public:
	// Constructor
	RelionJob() { clear(); };

	// Empty Destructor
	~RelionJob() { clear(); };

	// Clear everything
	void clear()
	{
		outputName = alias = "";
		type = -1;
		inputNodes.clear();
		outputNodes.clear();
		joboptions.clear();
		is_continue = false;
		do_tomo = false;
	}

	// Returns true if the option is present in joboptions
	bool containsLabel(std::string label, std::string &option);

	// Set this option in the job
	void setOption(std::string setOptionLine);

	// Activate specific options for tomo
	void setTomo(bool _do_tomo)
	{
		do_tomo = _do_tomo;
	}

	// write/read settings to disc
	// fn is a directory name (e.g. Refine3D/job123/) or a STAR file
	bool read(std::string fn, bool &_is_continue, bool do_initialise = false); // return false if unsuccessful
	void write(std::string fn);

	// Write the job submission script
	bool saveJobSubmissionScript(std::string newfilename, std::string outputname, std::vector<std::string> commands, std::string &error_message);

	// Initialise pipeline stuff for each job, return outputname
	void initialisePipeline(std::string &outputname, std::string defaultname, int job_counter);

	// Prepare the final (job submission or combined (mpi) command of possibly multiple lines)
	// Returns true to go ahead, and false to cancel
	bool prepareFinalCommand(std::string &outputname, std::vector<std::string> &commands, std::string &final_command,
			bool do_makedir, std::string &warning_message);

	// Initialise the generic RelionJob
	void initialise(int job_type);

	// Generic getCommands
	bool getCommands(std::string &outputname, std::vector<std::string> &commands,
	 		std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	// Now all the specific job types are defined
	void initialiseImportJob();
	bool getCommandsImportJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseMotioncorrJob();
	bool getCommandsMotioncorrJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseCtffindJob();
	bool getCommandsCtffindJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseManualpickJob();
	bool getCommandsManualpickJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseAutopickJob();
	bool getCommandsAutopickJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseExtractJob();
	bool getCommandsExtractJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseSelectJob();
	bool getCommandsSelectJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseClass2DJob();
	bool getCommandsClass2DJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseInimodelJob();
	bool getCommandsInimodelJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseClass3DJob();
	bool getCommandsClass3DJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseAutorefineJob();
	bool getCommandsAutorefineJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseMultiBodyJob();
	bool getCommandsMultiBodyJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseMaskcreateJob();
	bool getCommandsMaskcreateJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseJoinstarJob();
	bool getCommandsJoinstarJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseSubtractJob();
	bool getCommandsSubtractJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialisePostprocessJob();
	bool getCommandsPostprocessJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseLocalresJob();
	bool getCommandsLocalresJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseMotionrefineJob();
	bool getCommandsMotionrefineJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseCtfrefineJob();
	bool getCommandsCtfrefineJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseExternalJob();
	bool getCommandsExternalJob(std::string &outputname, std::vector<std::string> &commands,
			std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);


	// relion-3.2: add subtomogram averaging programs by Jasenko
	void addTomoInputOptions(bool has_tomograms, bool has_particles,
							 bool has_trajectories, bool has_manifolds, bool has_halfmaps, bool has_postprocess);
	std::string getTomoInputCommmand(std::string &command, int has_tomograms, int has_particles,
									 int has_trajectories, int has_manifolds, bool has_halfmaps, int has_postprocess);

	std::string setTomoOutputCommand(std::string &command, std::string optimisationSet, std::string tomograms,
									 std::string particles, std::string trajectories, std::string manifolds,
									 std::string halfmap1, std::string postprocess, std::string refmask,
									 std::string optimisationSetOut);

	void initialiseTomoImportJob();
	bool getCommandsTomoImportJob(std::string &outputname, std::vector<std::string> &commands,
								  std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseTomoSubtomoJob();
	bool getCommandsTomoSubtomoJob(std::string &outputname, std::vector<std::string> &commands,
								   std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseTomoCtfRefineJob();
	bool getCommandsTomoCtfRefineJob(std::string &outputname, std::vector<std::string> &commands,
									 std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseTomoAlignJob();
	bool getCommandsTomoAlignJob(std::string &outputname, std::vector<std::string> &commands,
								 std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

	void initialiseTomoReconPartJob();
	bool getCommandsTomoReconPartJob(std::string &outputname, std::vector<std::string> &commands,
									 std::string &final_command, bool do_makedir, int job_counter, std::string &error_message);

};

#endif /* SRC_PIPELINE_JOBS_H_ */
