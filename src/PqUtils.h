#ifndef __RSQLITE_PQ_UTILS__
#define __RSQLITE_PQ_UTILS__

#include <Rcpp.h>
#include <libpq-fe.h>

// Defined in encode.cpp -------------------------------------------------------

void escapeInBuffer(const char* string, std::string& buffer);
void encodeInBuffer(Rcpp::RObject x, int i, std::string& buffer);
void encodeRowInBuffer(Rcpp::List x, int i, std::string& buffer,
  std::string fieldDelim = "\t",
  std::string lineDelim = "\n");
std::string encode_data_frame(Rcpp::List x);

// Generic data frame utils ----------------------------------------------------

enum class PGTypes {
  Int = INTSXP,
  Real = REALSXP,
  String = STRSXP,
  Logical = LGLSXP,
  Vector = VECSXP,
  Date,
  Datetime,
  DatetimeTZ,
  Time
};

Rcpp::List inline dfResize(Rcpp::List df, int n) {
  int p = df.size();

  Rcpp::List out(p);
  for (int j = 0; j < p; ++j) {
    out[j] = Rf_lengthgets(df[j], n);
  }

  out.attr("names") = df.attr("names");
  out.attr("class") = df.attr("class");
  out.attr("row.names") = Rcpp::IntegerVector::create(NA_INTEGER, -n);

  return out;
}

Rcpp::List inline dfCreate(const std::vector<PGTypes>& types, const std::vector<std::string>& names, int n) {
  int p = types.size();

  Rcpp::List out(p);
  out.attr("names") = names;
  out.attr("class") = "data.frame";
  out.attr("row.names") = Rcpp::IntegerVector::create(NA_INTEGER, -n);

  int j = 0;
  for (auto it = types.begin(); it != types.end(); ++it, j++) {
      switch (*it) {
      case PGTypes::Date:
          out[j] = Rf_allocVector(INTSXP, n);
          break;
      case PGTypes::Datetime:
      case PGTypes::DatetimeTZ:
      case PGTypes::Time:
          out[j] = Rf_allocVector(REALSXP, n);
          break;
      default:
          out[j] = Rf_allocVector(static_cast<SEXPTYPE>(*it), n);
      }
  }
  return out;
}

#endif
