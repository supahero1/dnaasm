/**
 * \file Output.hpp
 * \brief the C++ file with implementation of template class for generating output from set of statistics and sequences
 *
 */

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <fstream>
#include <iostream>
#include "Statistics.hpp"
#include "ResultantSequencesGenerator.hpp"
#include "../common/log.hpp"

namespace dnaasm { namespace dbj {
    /**
     * \brief Class to generate output from set of statistics and resultant sequences.
     */
    template<class Graph> class Output {
    public:

        /**
         * \brief Output constructor.
         *
         * \param result application output
         * \param statistics reference to statistics object with set of statistics
         * \param resultantSequencesGenerator reference to resultantSequencesGenerator object with set of data from resultant sequences
         * \param idPrefix prefix to output FASTA format sequence's id line
         */
        Output(std::string& result,
               Statistics& statistics,
               ResultantSequencesGenerator<Graph>& resultantSequencesGenerator,
               std::string& idPrefix)
            : result_(result)
            , statistics_(statistics)
            , resultantSequencesGenerator_(resultantSequencesGenerator)
            , idPrefix_(idPrefix)
        {}

        /**
         * \brief Generate output from set of statistics and resultants data.
         *
         * \return returns void
         */
        void generateApplicationOutput() {
            logAssemblingStatistics();
            logSequencesStatistics();
            writeSequencesLengthStatistics();
            writeSequencesOutput();
        }

    private:
        std::string& result_;
        Statistics& statistics_;
        ResultantSequencesGenerator<Graph>& resultantSequencesGenerator_;
        std::string& idPrefix_;
        Output& operator=(const Output&)= delete;

        /**
         * \brief Generates output from set of resultant sequences.
         *
         * \return returns void
         */
        void writeSequencesOutput() {
            unsigned int i = 0;
            while (true) {
                if (resultantSequencesGenerator_.getOut(i) == "")
                    return;
                result_ += (">"
                        + idPrefix_
                        + "_" + std::to_string(i)
                        + "_" + std::to_string(resultantSequencesGenerator_.getOut(i).size())
                        + "\r\n" + resultantSequencesGenerator_.getOut(i)+ "\r\n");
                ++i;
            }
        }

        /**
         * \brief Generates length statistics for set of resultant sequences and writes them to file.
         *
         * \return returns void
         */
        void writeSequencesLengthStatistics() {
            std::ofstream ofile(CALC_TMP_DIR+std::string("sequencesStats"));
            ofile << "sum max 100 sequences: " << "   " << statistics_.getSumOfMaxSequences(100U) << std::endl;
            ofile << "sum max 500 sequences: " << "   " << statistics_.getSumOfMaxSequences(500U) << std::endl;
            ofile << "sum max 1000 sequences: " << "   " << statistics_.getSumOfMaxSequences(1000U) << std::endl;
            ofile << std::endl;
            ofile << "sum of sequences longer or equal to 100 bp: " << "   " << statistics_.getSumOfSequencesLongerOrEqualTo(100U) << std::endl;
            ofile << "sum of sequences longer or equal to 1000 bp: " << "   " << statistics_.getSumOfSequencesLongerOrEqualTo(1000U) << std::endl;
            ofile << "sum of sequences longer or equal to 10000 bp: " << "   " << statistics_.getSumOfSequencesLongerOrEqualTo(10000U) << std::endl;
            ofile.close();
        }

        /**
        * \brief Logs statistics for assembling process.
        *
        * \return returns void
        */
        void logAssemblingStatistics() {
            //logInfo("num of unfiltered reads: " + std::to_string(statistics_.getUnfilteredReadsHistogramNumOfElements()));
            //logInfo("num of filtered reads: " + std::to_string(statistics_.getFilteredReadsHistogramNumOfElements()));
            //logInfo("num of symbols in unfiltered reads: " + std::to_string(statistics_.getUnfilteredReadsHistogramSum()));
            //logInfo("num of symbols in filtered reads: " + std::to_string(statistics_.getFilteredReadsHistogramSum()));
            //logInfo("num of components of reads graph: " + std::to_string(boost::get<unsigned long>(statistics_.get(Statistics::Key::NUM_OF_COMPONENTS))));
        }

        /**
        * \brief Logs statistics for resultant sequences.
        *
        * \return returns void
        */
        void logSequencesStatistics() {
            std::cerr << "---" << std::endl;
            std::cerr << "num of sequences: " << boost::get<unsigned long>(statistics_.get(Statistics::Key::NUM_OF_SEQUENCES)) << std::endl;
            std::cerr << "sum: " << boost::get<unsigned long>(statistics_.get(Statistics::Key::SUM_SEQUENCES_SIZE)) << std::endl;
            std::cerr << "max: " << boost::get<unsigned long>(statistics_.get(Statistics::Key::MAX_SEQUENCE_SIZE)) << std::endl;
            std::cerr << "average: " << boost::get<float>(statistics_.get(Statistics::Key::AVG_SEQUENCE_SIZE)) << std::endl;
            std::cerr << "median: " << boost::get<float>(statistics_.get(Statistics::Key::MEDIAN_SEQUENCE_SIZE)) << std::endl;
            std::cerr << "N50: " << boost::get<unsigned long>(statistics_.get(Statistics::Key::N50_SEQUENCE_SIZE)) << std::endl;
            std::cerr << "---" << std::endl;
        }

    };

}}

#endif    // OUTPUT_HPP
