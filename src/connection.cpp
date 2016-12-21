#include <Rcpp.h>
#include "RPostgres_types.h"
using namespace Rcpp;

// [[Rcpp::export]]
XPtr<PqConnection> connection_create(std::vector<std::string> keys,
                                        std::vector<std::string> values) {
    PqConnection* pConn = new PqConnection(keys, values);
    return XPtr<PqConnection>(pConn, true);
}

// [[Rcpp::export]]
void connection_release(XPtr<PqConnection> con) {
  if (con->hasQuery()) {
    warning("%s\n%s",
      "There is a result object still in use.",
      "The connection will be automatically released when it is closed"
    );
  }
  con.release();
}

// [[Rcpp::export]]
List connection_info(XPtr<PqConnection> con) {
  return con->info();
}

// [[Rcpp::export]]
CharacterVector connection_escape_string(XPtr<PqConnection> con,
                                         CharacterVector xs) {
  int n = xs.size();
  CharacterVector escaped(n);

  for (int i = 0; i < n; ++i) {
    std::string x(xs[i]);
    escaped[i] = con->escapeString(x);
  }

  return escaped;
}

// [[Rcpp::export]]
CharacterVector connection_escape_identifier(XPtr<PqConnection> con,
                                             CharacterVector xs) {
  int n = xs.size();
  CharacterVector escaped(n);

  for (int i = 0; i < n; ++i) {
    std::string x(xs[i]);
    escaped[i] = con->escapeIdentifier(x);
  }

  return escaped;
}

// [[Rcpp::export]]
void connection_copy_data(XPtr<PqConnection> con, std::string sql, List df) {
  return con->copyData(sql, df);
}
