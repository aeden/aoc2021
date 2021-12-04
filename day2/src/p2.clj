(ns p2
  (:import (java.io BufferedReader FileReader))
  (:require [clojure.string :as str]))

(defn readall [filename]
  (with-open [rdr (BufferedReader. (FileReader. filename))]
    (doall (line-seq rdr))))

(defn commands [lines]
  (map (fn [line] (str/split line #" ")) lines))

(defn execute-next-command [acc commands aim]
  (if (empty? commands)
    acc
    (let [[action value] (first commands)]
      (case action
        "forward" (execute-next-command (conj acc [(Integer/parseInt value) (* aim (Integer/parseInt value))]) (rest commands) aim)
        "up" (execute-next-command acc (rest commands) (- aim (Integer/parseInt value)))
        "down" (execute-next-command acc (rest commands) (+ aim (Integer/parseInt value)))))))

(defn execute-commands [commands]
  (let [acc []]
    (execute-next-command acc commands 0)))

(defn calculate [pairs]
  (reduce * (reduce (fn [[ha da] [horizontal depth]] [(+ ha horizontal) (+ da depth)]) pairs)))

(defn run [opts]
  (let [{filename :filename} opts]
    (if (nil? filename)
      (println ":filename not specified")
      (println (calculate (execute-commands (commands (readall (str filename)))))))))
