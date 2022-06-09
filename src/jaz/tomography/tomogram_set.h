#ifndef TOMOGRAM_SET_H
#define TOMOGRAM_SET_H

#include <string>
#include <vector>
#include <src/metadata_table.h>
#include <src/filename.h>
#include <src/jaz/tomography/tomogram.h>
#include <src/jaz/gravis/t4Matrix.h>
#include <src/ctf.h>

class TomogramSet
{
	public:

        MetaDataTable globalTable;
        std::vector<MetaDataTable> tomogramTables;

        TomogramSet();
        TomogramSet(FileName filename, bool verbose = true);

        // return false if this is not a TomogramSet
        bool read(std::string filename, bool verbose = true);
        void write(FileName filename);

		Tomogram loadTomogram(int index, bool loadImageData) const;
			
		void addTomogramFromIMODStack(
			std::string tomoName, std::string stackFilename,
			const std::vector<gravis::d4Matrix>& projections,
			int w, int h, int d,
			const std::vector<double>& dose,
			double fractionalDose,
			const std::vector<CTF>& ctfs,
			double handedness, 
			double pixelSize);
		
		int size() const;

		void setProjections(int tomogramIndex, const std::vector<gravis::d4Matrix>& proj);
		void setProjection(int tomogramIndex, int frame, const gravis::d4Matrix& P);
		void setCtf(int tomogramIndex, int frame, const CTF& ctf);
		void setDose(int tomogramIndex, int frame, double dose);
		void setTiltSeriesFile(int tomogramIndex, const std::string& filename);
		void setFiducialsFile(int tomogramIndex, const std::string& filename);
		void setDefocusSlope(int tomogramIndex, double slope);

		void setDeformation(
				int tomogramIndex,
				gravis::i2Vector gridSize,
				const std::string& deformationType,
				const std::vector<std::vector<double>>& coeffs);

		void clearDeformation();

		int getTomogramIndex(std::string tomogramName) const;
        std::string getTomogramName(int index) const;
		int getTomogramIndexSafely(std::string tomogramName) const;
		int getFrameCount(int index) const;
		int getMaxFrameCount() const;
		double getPixelSize(int index) const;
		std::string getOpticsGroupName(int index) const;

        // SHWS 6Apr2022: Make one big metadatatable with all movies/micrographs (to be used for motioncorr and ctffind runners)
        void generateSingleMetaDataTable(MetaDataTable &MDout, ObservationModel &obsModel);

        // SHWS 6Apr2022: Convert back from one big metadatatable into separate STAR files for each tilt serie
        void convertBackFromSingleMetaDataTable(MetaDataTable &MDin);

};

#endif
