#include "types.h"

#include "FeatureSet.h"
#include "Summary.h"

#include "SignificantFeaturesSearch.h"
#include "SignificantItemsetSearch.h"
#include "SignificantItemsetSearchFacs.h"
#include "SignificantIntervalSearch.h"
#include "SignificantIntervalSearchFais.h"
#include "SignificantIntervalSearchExact.h"
#include "SignificantIntervalSearchChi.h"
#include "SignificantIntervalSearchWy.h"
#include "SignificantIntervalSearchWyExact.h"
#include "SignificantIntervalSearchWyChi.h"
#include "SignificantFeaturesSearchWithCovariates.h"
#include "SignificantIntervalSearchFastCmh.h"

#include <Rcpp.h>
using namespace Rcpp;

//TODO: use Rcpp modules to create S4/RefClasses directly; cf.
//      http://dirk.eddelbuettel.com/code/rcpp/Rcpp-modules.pdf

SEXP _get_intervals(std::vector<SignificantPattern::Interval>& intervals) {
    size_t size = intervals.size();

    IntegerVector starts(size);
    IntegerVector ends(size);
    DoubleVector pvalues(size);

    for (size_t i = 0; i < size; i++) {
        starts[i] = intervals[i].getStart();
        ends[i] = intervals[i].getEnd();
        pvalues[i] = intervals[i].getPvalue();
    }

    return DataFrame::create(Named("start") = starts, Named("end") = ends,
                             Named("pvalue") = pvalues);
}

SEXP _get_itemsets(const SignificantPattern::ItemsetSet itemsets) {

    size_t size = itemsets.getLength();
    const std::vector<double> pvals  = itemsets.getPValueVector();
    const std::vector< std::vector<longint> > itemsets_vec = itemsets.getItemsetsVector();
    IntegerVector rownamesVec(size);
    // NumericVector pvalsVec(size);
    // List itemsetsList(size);
    // size_t iset_size;
    for (size_t i = 0; i < size; ++i) {
        // pvalsVec[i] = pvals[i];
        // std::vector<longint> itemset_vec = itemsets_vec[i];
        // iset_size = itemset_vec.size();
        // IntegerVector itemsetVec(iset_size);
        // // fprintf(stdout, "\tiset_size=%zu\n", iset_size);
        // for(int j = 0; j < iset_size; ++j)
        //     itemsetVec[j] = itemset_vec[j];
        // itemsetsList[i] = itemsetVec;
        rownamesVec[i] = i;
    }

    // Hack: create data frame from a list by a class atrr change, because
    // as.data.frame (or data.frame) R functions try to convert items in list
    // elements to separate columns
    // List ret = List::create(Named("itemsets") = itemsetsList,
    //                         Named("pvalue") =  pvalsVec);
    List ret =  List::create(Named("itemsets") = itemsets_vec,
                             Named("pvalue") =  pvals);
    ret.attr("class") = "data.frame";
    ret.attr("row.names") = rownamesVec;
    return ret;
}

// [[Rcpp::export]]
SEXP lib_new_search_e() {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchExact> ptr( new SignificantPattern::SignificantIntervalSearchExact(), true );
    return ptr;
}

// [[Rcpp::export]]
SEXP lib_new_search_chi() {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchChi> ptr( new SignificantPattern::SignificantIntervalSearchChi(), true );
    return ptr;
}

// [[Rcpp::export]]
SEXP lib_new_search_wy_e() {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWyExact> ptr( new SignificantPattern::SignificantIntervalSearchWyExact(), true );
    return ptr;
}

// [[Rcpp::export]]
SEXP lib_new_search_wy_chi() {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWyChi> ptr( new SignificantPattern::SignificantIntervalSearchWyChi(), true );
    return ptr;
}

// [[Rcpp::export]]
SEXP lib_new_search_fastcmh() {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFastCmh> ptr( new SignificantPattern::SignificantIntervalSearchFastCmh(), true );
    return ptr;
}

// [[Rcpp::export]]
SEXP lib_new_search_facs() {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearchFacs> ptr( new SignificantPattern::SignificantItemsetSearchFacs(), true );
    return ptr;
}

// [[Rcpp::export]]
void lib_delete_search_e(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchExact> ptr(inst);
    ptr.release();
}

// [[Rcpp::export]]
void lib_delete_search_chi(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchChi> ptr(inst);
    ptr.release();
}

// [[Rcpp::export]]
void lib_delete_search_wy_e(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWyExact> ptr(inst);
    ptr.release();
}

// [[Rcpp::export]]
void lib_delete_search_wy_chi(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWyChi> ptr(inst);
    ptr.release();
}

// [[Rcpp::export]]
void lib_delete_search_fastcmh(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFastCmh> ptr(inst);
    ptr.release();
}

// [[Rcpp::export]]
void lib_delete_search_facs(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearchFacs> ptr(inst);
    ptr.release();
}

// Because of multiple inheritance, a first non-virtual class that actually
// constructs the virtual public class SignificantFeaturesSearch has to be used
// for calling SignificantFeaturesSearch non-virtual methods; hence, duplicates.
// Moreover, for overriden methods with covariant (return) types, we have to
// declare a separate function for each different subtype, as for summaries, or
// results.

// [[Rcpp::export]]
void lib_read_eth_files(SEXP inst, std::string x_filename, std::string y_filename) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    ptr->readETHFiles(x_filename, y_filename);
}

//TODO: read_eth_files_{int,iset}

// // [[Rcpp::export]]
// void read_eth_files_int(SEXP inst, std::string x_filename, std::string y_filename) {
//     Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
//     ptr->readETHFiles(x_filename, y_filename);
// }

// // [[Rcpp::export]]
// void read_eth_files_iset(SEXP inst, std::string x_filename, std::string y_filename) {
//     Rcpp::XPtr<SignificantPattern::SignificantItemsetSearch> ptr(inst);
//     ptr->readETHFiles(x_filename, y_filename);
// }

// [[Rcpp::export]]
void lib_read_eth_files_with_cov_fastcmh(SEXP inst, std::string x_filename, std::string y_filename, std::string covfilename) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFastCmh> ptr(inst);
    ptr->readETHFilesWithCovariates(x_filename, y_filename, covfilename);
}
// [[Rcpp::export]]
void lib_read_eth_files_with_cov_facs(SEXP inst, std::string x_filename, std::string y_filename, std::string covfilename) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearchFacs> ptr(inst);
    ptr->readETHFilesWithCovariates(x_filename, y_filename, covfilename);
}

// [[Rcpp::export]]
void lib_read_plink_files(SEXP inst, std::string base_filename) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    ptr->readPlinkFiles(base_filename);
}

//TODO: read_plink_files_{int,iset}

// // [[Rcpp::export]]
// void read_plink_files_int(SEXP inst, std::string base_filename) {
//     Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
//     ptr->readPlinkFiles(base_filename);
// }

// // [[Rcpp::export]]
// void read_plink_files_iset(SEXP inst, std::string base_filename) {
//     Rcpp::XPtr<SignificantPattern::SignificantItemsetSearch> ptr(inst);
//     ptr->readPlinkFiles(base_filename);
// }

// [[Rcpp::export]]
void lib_read_plink_files_with_cov_fastcmh(SEXP inst, std::string base_filename, std::string covfilename) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFastCmh> ptr(inst);
    ptr->readPlinkFilesWithCovariates(base_filename, covfilename);
}
// [[Rcpp::export]]
void lib_read_plink_files_with_cov_facs(SEXP inst, std::string base_filename, std::string covfilename) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearchFacs> ptr(inst);
    ptr->readPlinkFilesWithCovariates(base_filename, covfilename);
}

// [[Rcpp::export]]
void lib_read_covariates_file_fastcmh(SEXP inst, std::string cov_filename) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFastCmh> ptr(inst);
    ptr->readCovariatesFile(cov_filename);
}
// [[Rcpp::export]]
void lib_read_covariates_file_facs(SEXP inst, std::string cov_filename) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearchFacs> ptr(inst);
    ptr->readCovariatesFile(cov_filename);
}

// [[Rcpp::export]]
void lib_write_eth_files_iset(SEXP inst, std::string x_filename, std::string y_filename) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearch> ptr(inst);
    ptr->writeETHFiles(x_filename, y_filename);
}
// [[Rcpp::export]]
void lib_write_eth_files_int(SEXP inst, std::string x_filename, std::string y_filename) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    ptr->writeETHFiles(x_filename, y_filename);
}

// [[Rcpp::export]]
void lib_write_eth_files_with_cov_fastcmh(SEXP inst, std::string x_filename, std::string y_filename, std::string covfilename) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFastCmh> ptr(inst);
    ptr->writeETHFilesWithCovariates(x_filename, y_filename, covfilename);
}

// [[Rcpp::export]]
void lib_write_eth_files_with_cov_facs(SEXP inst, std::string x_filename, std::string y_filename, std::string covfilename) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearchFacs> ptr(inst);
    ptr->writeETHFilesWithCovariates(x_filename, y_filename, covfilename);
}

// [[Rcpp::export]]
void lib_execute_iset(SEXP inst, double alpha, longint l_max) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearch> ptr(inst);
    ptr->execute(alpha, l_max);
}
// [[Rcpp::export]]
void lib_execute_int(SEXP inst, double alpha, longint l_max) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    ptr->execute(alpha, l_max);
}

// [[Rcpp::export]]
void lib_summary_write_to_file_fais(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFais> ptr(inst);
    ptr->getSummary().writeToFile(output_file);
}
// [[Rcpp::export]]
void lib_summary_write_to_file_wy(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWy> ptr(inst);
    ptr->getSummary().writeToFile(output_file);
}
// [[Rcpp::export]]
void lib_summary_write_to_file_fastcmh(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFastCmh> ptr(inst);
    ptr->getSummary().writeToFile(output_file);
}
// [[Rcpp::export]]
void lib_summary_write_to_file_facs(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearchFacs> ptr(inst);
    ptr->getSummary().writeToFile(output_file);
}

// [[Rcpp::export]]
void lib_profiler_write_to_file(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    ptr->getProfiler().writeToFile(output_file);
}
//TODO: profiler_write_to_file_{int,iset}

// [[Rcpp::export]]
void lib_filter_intervals_write_to_file(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    ptr->getFilteredIntervals().writeToFile(output_file);
}

// [[Rcpp::export]]
void lib_pvals_testable_ints_write_to_file(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    ptr->getPValsTestableInts().writeToFile(output_file);
}

// [[Rcpp::export]]
void lib_pvals_significant_ints_write_to_file(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    ptr->getPValsSigInts().writeToFile(output_file);
}

// [[Rcpp::export]]
void lib_pvals_testable_isets_write_to_file(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearch> ptr(inst);
    ptr->getPValsTestableIsets().writeToFile(output_file);
}

// [[Rcpp::export]]
void lib_pvals_significant_isets_write_to_file(SEXP inst, std::string output_file) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearch> ptr(inst);
    ptr->getPValsSigIsets().writeToFile(output_file);
}

// [[Rcpp::export]]
SEXP lib_get_significant_intervals(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    return _get_intervals(ptr->getSignificantIntervals().getSigInts());
}

// [[Rcpp::export]]
SEXP lib_get_filtered_intervals(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    return _get_intervals(ptr->getFilteredIntervals().getSigInts());
}

// [[Rcpp::export]]
SEXP lib_get_significant_itemsets(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearch> ptr(inst);
    return _get_itemsets(ptr->getPValsSigIsets());
}

// [[Rcpp::export]]
longint lib_get_n_perm_wy(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWy> ptr(inst);
    return ptr->getNPerm();
}

// [[Rcpp::export]]
void lib_set_n_perm_wy(SEXP inst, longint n_perm) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWy> ptr(inst);
    ptr->setNPerm(n_perm);
}

// [[Rcpp::export]]
void lib_set_perm_file_name_wy(SEXP inst, std::string filename) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWy> ptr(inst);
    ptr->setPermutationsFilename(filename);
}

// [[Rcpp::export]]
void lib_set_seed_wy(SEXP inst, unsigned s) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchWy> ptr(inst);
    ptr->setSeed(s);
}

// [[Rcpp::export]]
SEXP lib_get_result_fais(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearchFais> ptr(inst);
    SignificantPattern::SummaryFais sum = ptr->getSummary();

    SEXP start = IntegerVector::create(sum.getSl1(), sum.getSu1());
    SEXP end = IntegerVector::create(sum.getSl2(), sum.getSu2());
    SEXP region = List::create(Named("start") = start,
                               Named("end") = end);

    return List::create(Named("sig.int") = lib_get_significant_intervals(inst),
                        Named("sig.int.clustered") = lib_get_filtered_intervals(inst),
                        Named("int.processed") = sum.getNumFeaturesProcessed(),
                        Named("int.testable") = sum.getm(),
                        Named("testability.region") = region,
                        Named("testability.threshold") = sum.getDelta(),
                        Named("significance.level") = sum.getAlpha(),
                        Named("corrected.significance.level") = sum.getDelta_opt());
}

// [[Rcpp::export]]
SEXP lib_get_result_int(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantIntervalSearch> ptr(inst);
    SignificantPattern::SummaryInt sum = ptr->getSummary();

    return List::create(Named("sig.int") = lib_get_significant_intervals(inst),
                        Named("sig.int.clustered") = lib_get_filtered_intervals(inst),
                        Named("int.processed") = sum.getNumFeaturesProcessed(),
                        Named("int.testable") = sum.getm(),
                        Named("testability.threshold") = sum.getDelta(),
                        Named("significance.level") = sum.getAlpha(),
                        Named("corrected.significance.level") = sum.getDelta_opt());
}

// [[Rcpp::export]]
SEXP lib_get_result_iset(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearch> ptr(inst);
    SignificantPattern::SummaryIset sum = ptr->getSummary();

    return List::create(Named("sig.iset") = lib_get_significant_itemsets(inst),
                        Named("iset.processed") = sum.getNumFeaturesProcessed(),
                        Named("iset.testable") = sum.getm(),
                        Named("testability.threshold") = sum.getDelta(),
                        Named("significance.level") = sum.getAlpha(),
                        Named("corrected.significance.level") = sum.getDelta_opt());
}

// [[Rcpp::export]]
SEXP lib_get_result_facs(SEXP inst) {
    Rcpp::XPtr<SignificantPattern::SignificantItemsetSearchFacs> ptr(inst);
    SignificantPattern::SummaryFacs sum = ptr->getSummary();

    return List::create(Named("sig.iset") = lib_get_significant_itemsets(inst),
                        Named("iset.processed") = sum.getNumFeaturesProcessed(),
                        Named("iset.closed.processed") = sum.getNumItemsetsClosedProcessed(),
                        Named("iset.testable") = sum.getm(),
                        Named("testability.threshold") = sum.getDelta(),
                        Named("significance.level") = sum.getAlpha(),
                        Named("corrected.significance.level") = sum.getDelta_opt());
}
