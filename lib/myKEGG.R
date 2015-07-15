library("RISmed")
library("KEGGgraph")
library("KEGGREST")

download_kegg <- function(id) {
  tmp = tempfile()
  retrieveKGML(pathwayid=substr(id, 4, 8), organism=substr(id, 1, 3), destfile=tmp, method="wget")
  pathway <- parseKGML(tmp)
}

parse_genes <- function(kegg) {
  nodes <- nodes(pathway)
  genes <- nodes[sapply(nodes, getType) == "gene"]
}

get_names <- function(genes) {
  a = sapply(genes, getName)
}

get_aliases <- function(genes) {
  a = sapply(genes, getDisplayName)
  a = gsub("\\.", "", a)
}

get_all_genes <- function(test) {
  a = get_names(p_genes)
}

grab_gene_names <- function(x) {
  a = sapply(x, all_to_ind)
}

temp <- function(test) {
  a = test$NAME
}

all_to_ind <- function(x) {
  a = unlist(x)
  a = paste(sapply(kegg_get_ind(a), temp), collapse=", ")
}

kegg_get_ind <- function(x) {
  a = sapply(x, keggGet)
}

make_OR <- function(x) {
  a = paste("(\"", gsub(", ", "\"[tiab] OR \"", x), "\"[tiab])", sep = "")
}

make_AND <- function(x1, x2) {
  a = paste("(", x1, " AND ", x2 ,")", sep = "")
}

create_queries <- function(x) {
  queries = vector()
  size = length(x)
  #ddsapply(1:26, function(i) { queries <- append(queries, i) } )
  for (i in 1:(size - 1)) {
    for (j in (i + 1):size) {
      if (i != j) {
        queries = c(queries, make_AND(make_OR(x[i]), make_OR(x[j])))
      }
    }
  }
  queries
}

create_queries2 <- function(x) {
  all_ids = vector()
  queries = vector()
  size = 3
  #ddsapply(1:26, function(i) { queries <- append(queries, i) } )
  for (i in 1:(size - 1)) {
    for (j in (i + 1):size) {
      if (i != j) {
        q = (make_AND(make_OR(x[i]), make_OR(x[j])))
        ngs_search <- EUtilsSummary(x, type="esearch",db = "pubmed")
        ids = QueryId(ngs_search)
        print(ids)
      }
    }
  }
  aid = all_ids
}

query_id <- function(x) {
  Sys.sleep(0.333)
  ngs_search <- EUtilsSummary(x, type="esearch",db = "pubmed")
  ids = QueryId(ngs_search)
}

query_ids <- function(x) {
  a = lapply(x, query_id)
}

get_ids_from_query <- function(x) {
  a = x[[1]]
}

break_down_ids <- function(x) {
  a = vector()
  if (length(x) > 0) {
    a = x[1]
  }
  a
}

get_data <- function(x) {
  pathway <<- download_kegg(x)
}

check_abstracts <- function(ids, genes) {
    maxnum = length(ids)
    full_mat = matrix(1, 0, length(genes))
    ab <- vector()
    for (i in 0:(length(ids)/200)) {
        from = (i * 200) + 1
        to = (from + 199)
        to = min(to, maxnum)
        message(paste("Downloading Abstracts ", from, " to " , to))
        query = paste(paste(ids[from:to], collapse="[UID] OR "),"[UID]", sep="")
        ngs_search <- EUtilsSummary(query, type="esearch",db = "pubmed")
        #print(query)
        Sys.sleep(0.333)
        ngs_record <- EUtilsGet(ngs_search)
        ab = AbstractText(ngs_record)

        for (i in 1:length(ab)) {
          mat = matrix(1, 1, length(genes))
          for (j in 1:length(genes)) {
            if (grepl(gsub(', ', '|', genes[j]), ab[i])) {
              mat[1, j] = 2
            }
          }

          full_mat = rbind(full_mat, mat)
        }

        #out <<- ((sapply(genes, regexpr, ab, ignore.case=TRUE, USE.NAMES=FALSE) > 0) + 1)
        #file = paste0(paste(from, to, sep="_"), ".csv")
        #write.table(out, "abmat.csv", row.names=FALSE, col.names=FALSE, sep=",", append=TRUE)
    }
    return(full_mat)
}
