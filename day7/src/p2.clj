(ns p2
  (:import (java.io BufferedReader FileReader))
  (:require [clojure.string :as str]))

(defn readall [filename]
  (with-open [rdr (BufferedReader. (FileReader. filename))]
    (doall (line-seq rdr))))

(defn str-to-nums [s]
  (map #(Integer/parseInt %) (str/split s #",")))

(defn freq-map [positions]
   (frequencies positions))

(defn locations [positions]
  (range (apply min positions) (inc (apply max positions))))

(defn dist [start dest]
  (Math/abs (- start dest)))

(defn cost [start dest]
  (/ (* (dist start dest) (+ (dist start dest) 1)) 2))

(defn sum-of-crab-travel [loc fmap]
  (reduce + (map (fn [[start-pos crab-count]] (* (cost start-pos loc) crab-count) ) fmap)))

(defn least-fuel-spent [locations fmap]
  (apply min (map (fn [loc] (sum-of-crab-travel loc fmap)) locations)))

(defn run [opts]
  (let [{filename :filename} opts]
    (if (nil? filename)
      (println ":filename not specified")
      (let [start (sort (str-to-nums (first (readall (str filename)))))]
        (println "Least fuel spent:" (least-fuel-spent (locations start) (freq-map start)))
        ))))
