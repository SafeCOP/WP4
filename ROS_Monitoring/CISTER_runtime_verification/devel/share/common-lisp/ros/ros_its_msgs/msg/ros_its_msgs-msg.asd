
(cl:in-package :asdf)

(defsystem "ros_its_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "CAM_simplified" :depends-on ("_package_CAM_simplified"))
    (:file "_package_CAM_simplified" :depends-on ("_package"))
  ))