;;;;
(let ((lib (load-foreign "build/libread_shm_func.so")))
  (defforeign open-shm lib "open_shm" (:integer :integer) :integer)

  (defforeign read-ref-angle lib "readPositionCommand" (:integer :string) :integer)
  (defforeign read-cur-angle lib "readPositionCurrent" (:integer :string) :integer)
  (defforeign write-ref-angle lib "writePositionCommand" (:integer :string) :integer)
  (defforeign write-cur-angle lib "writePositionCurrent" (:integer :string) :integer)
  )

#|
(open-shm 8888 18)
(setq ref-angles (instantiate float-vector 18))
(setq cur-angles (instantiate float-vector 18))
(setq buf (instantiate float-vector 18))

(setf (aref ref-angles 0) 1.0)

(read-ref-angle (length buf) buf)

(write-ref-angle (length ref-angles) ref-angles)
(write-cur-angle (length cur-angles) cur-angles)
|#
;;;;
