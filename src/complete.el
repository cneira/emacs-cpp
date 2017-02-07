(require 'popup)
(require 'mymod)


(defun complete-at-point()
  (interactive)
  (let* (  (f (buffer-file-name (current-buffer)))
	   (col (current-column))
	   (line  (car (cdr (split-string (what-line) ) )   )  )
           (completions (mymod-test f (string-to-number line) col))
	   )
    (popup-menu* (split-string completions "\n"))
;;    (save-excursion (insert (popup-menu* (split-string completions "\n"))))
    ))

(global-set-key (kbd "<f9>") 'complete-at-point) 



