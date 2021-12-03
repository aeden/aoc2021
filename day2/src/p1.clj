(ns p1
  (:import (java.io BufferedReader FileReader))
  (:require [clojure.string :as str]))

(defn readall [filename]
  (with-open [rdr (BufferedReader. (FileReader. filename))]
    (doall (line-seq rdr))))

(defn commands [lines]
  (map (fn [line] (str/split line #" ")) lines))

(defn calculate [commands]
  (reduce * 
    (reduce (fn [[ha da] [horizontal depth]] [(+ ha horizontal) (+ da depth)])
      (map (fn [[command value]]
         (case command
           "forward" [(Integer/parseInt value) 0]
           "up" [0 (- (Integer/parseInt value))]
           "down" [0 (Integer/parseInt value)]
         )) commands))))
      
(defn run [opts]
  (let [{filename :filename} opts]
    (if (nil? filename)
      (println ":filename not specified")
      (println (calculate (commands (readall (str filename))))))))
