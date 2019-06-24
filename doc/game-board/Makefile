
all: game-board-docs.pdf

%.pdf: %.tex
	mkdir -p build
	pdflatex -output-directory build -halt-on-error $<
	pdflatex -output-directory build -halt-on-error $<
	cp build/$@ .
