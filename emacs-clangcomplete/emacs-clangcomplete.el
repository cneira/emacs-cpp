(require 'popup)
(require 'emacs-clangcomplete)


(defun current-line ()
  "Return the vertical position of point..."
  (+ (count-lines (point-min) (point))
     (if (= (current-column) 0) 1 0)
     -1))



(defun complete-at-point()
  (interactive)
  (let* (  (  f (buffer-file-name (current-buffer)) ) 
	   (comp (file-name-directory (buffer-file-name (current-buffer)) ) )
	   (col (+ 1 (current-column)) )
	   (line (+ 1 (current-line )))
           (completions (clang-complete f  line col comp    ))
	   )
    (popup-menu* (split-string completions "\n"))
;;    (save-excursion (insert (popup-menu* (split-string completions "\n"))))
    ))

(global-set-key (kbd "<f9>") 'complete-at-point) 




