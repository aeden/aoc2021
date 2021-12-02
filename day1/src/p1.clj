(ns p1
  (:import (java.io BufferedReader FileReader)))

(defn readall [filename]
  (with-open [rdr (BufferedReader. (FileReader. filename))]
    (doall (line-seq rdr))))
      
(defn strings-to-ints [values]
  (map #(Integer/parseInt %) values))

(defn pairs [values]
  (map vector values (rest values)))

(defn sum [pairs]
  (reduce + (map (fn [[a b]] (if (> b a) 1 0)) pairs)))


(defn run [opts]
  (let [{filename :filename} opts]
    (println (sum (pairs (strings-to-ints (readall (str filename))))))))
