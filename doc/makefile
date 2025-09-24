# Makefile for generating presentation slides from markdown
# By default uses beamer for PDF slides, but also supports other formats

# Variables
SRC = slides.md
PDF_OUTPUT = slides.pdf
PPTX_OUTPUT = slides.pptx
HTML_OUTPUT = slides.html

# Default target: generate PDF slides using beamer
all: pdf

# Generate PDF slides (Beamer)
pdf: $(SRC)
	@echo "Generating PDF presentation slides..."
	pandoc -t beamer $(SRC) -o $(PDF_OUTPUT)
	@echo "PDF slides created at $(PDF_OUTPUT)"

# Generate PowerPoint slides
pptx: $(SRC)
	@echo "Generating PowerPoint presentation..."
	pandoc -t pptx $(SRC) -o $(PPTX_OUTPUT)
	@echo "PowerPoint slides created at $(PPTX_OUTPUT)"

# Generate HTML slides (Slidy)
html: $(SRC)
	@echo "Generating HTML presentation..."
	pandoc -t slidy --self-contained $(SRC) -o $(HTML_OUTPUT)
	@echo "HTML slides created at $(HTML_OUTPUT)"

# Generate all slide formats
all-formats: pdf pptx html
	@echo "All presentation formats generated"

# Clean up generated files
clean:
	@echo "Cleaning up generated files..."
	rm -f $(PDF_OUTPUT) $(PPTX_OUTPUT) $(HTML_OUTPUT)
	@echo "Clean complete"

# Help message
help:
	@echo "Makefile for generating presentation slides from markdown using pandoc"
	@echo ""
	@echo "Available targets:"
	@echo "  all (default): Generate PDF slides using beamer"
	@echo "  pdf:          Generate PDF slides using beamer"
	@echo "  pptx:         Generate PowerPoint slides"
	@echo "  html:         Generate HTML slides (Slidy)"
	@echo "  all-formats:  Generate all supported slide formats"
	@echo "  clean:        Remove all generated files"
	@echo ""
	@echo "Example usage:"
	@echo "  make                # Generate PDF slides"
	@echo "  make pptx           # Generate PowerPoint slides"
	@echo "  make all-formats    # Generate all formats"

.PHONY: all pdf pptx html all-formats clean help