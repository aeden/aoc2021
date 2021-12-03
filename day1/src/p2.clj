(ns p2
  (:import (java.io BufferedReader FileReader)))

(defn readall [filename]
  (with-open [rdr (BufferedReader. (FileReader. filename))]
    (doall (line-seq rdr))))
      
(defn strings-to-ints [values]
  (map #(Integer/parseInt %) values))

(defn triples [values]
  (map vector values (rest values) (rest (rest values))))

(defn sum-triples [triples]
  (map (fn [triples] (reduce + triples)) triples))

(defn pairs [values]
  (map vector values (rest values)))

(defn sum [pairs]
  (reduce + (map (fn [[a b]] (if (> b a) 1 0)) pairs)))

(defn run [opts]
  (let [{filename :filename} opts]
    (if (nil? filename)
      (println ":filename not specified")
      (println (sum (pairs (sum-triples (triples (strings-to-ints (readall (str filename)))))))))))
